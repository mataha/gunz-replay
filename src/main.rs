use std::env::args;
use std::ffi::OsStr;
use std::fmt;
use std::io::Cursor;
use std::io::Read;
use std::mem::size_of;
use std::path::Path;

use byteorder::LittleEndian;
use byteorder::ReadBytesExt;
use encoding_rs::WINDOWS_1252;

use file::read;

mod file;

#[derive(Clone, Copy, Debug)]
#[repr(C, packed)]
struct LegacyStageSetting {
    uid: u64,
    map: [u8; 32],
    map_index: i32,
    game_type: i32,
    max_rounds: i32,
    time_limit: i32,
    level_limit: i32,
    max_players: i32,
    friendly_fire: bool,
    point_victory: bool,
    forced_entry: bool,
    padding: u8,
}

impl LegacyStageSetting {
    fn new(bytes: &Vec<u8>, start: usize) -> LegacyStageSetting {
        let mut reader = Cursor::new(&bytes[start..size_of::<LegacyStageSetting>() + start]);
        let mut map: [u8; 32] = [0; 32];

        let uid = reader.read_u64::<LittleEndian>().unwrap();
        reader.read_exact(&mut map).unwrap();
        let map_index = reader.read_i32::<LittleEndian>().unwrap();
        let game_type = reader.read_i32::<LittleEndian>().unwrap();
        let max_rounds = reader.read_i32::<LittleEndian>().unwrap();
        let time_limit = reader.read_i32::<LittleEndian>().unwrap();
        let level_limit = reader.read_i32::<LittleEndian>().unwrap();
        let max_players = reader.read_i32::<LittleEndian>().unwrap();
        let friendly_fire = reader.read_u8().unwrap() != 0;
        let point_victory = reader.read_u8().unwrap() != 0;
        let forced_entry = reader.read_u8().unwrap() != 0;
        let padding = reader.read_u8().unwrap();

        LegacyStageSetting {
            uid,
            map,
            map_index,
            game_type,
            max_rounds,
            time_limit,
            level_limit,
            max_players,
            friendly_fire,
            point_victory,
            forced_entry,
            padding,
        }
    }
}

impl fmt::Display for LegacyStageSetting {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let (name, _, _) = WINDOWS_1252.decode(&self.map);

        write!(
            f,
            "{}, {} player game",
            name.trim_matches(char::from(0)),
            unsafe { &self.max_players }
        )
    }
}

#[derive(Clone, Copy, Debug)]
#[repr(C, packed)]
struct LegacyCharacterInfo {
    pov: bool,
    name: [u8; 32],
    clan: [u8; 16],
    grade: i32,
    contribution: u16,
    index: u8,
    level: u16,
    will_do_the_rest_later: [u8; 0x16f], // lol
}

impl LegacyCharacterInfo {
    fn new(bytes: &Vec<u8>, start: usize) -> LegacyCharacterInfo {
        let mut reader = Cursor::new(&bytes[start..size_of::<LegacyCharacterInfo>() + start]);
        let mut name: [u8; 32] = [0; 32];
        let mut clan: [u8; 16] = [0; 16];

        let pov = reader.read_u8().unwrap() != 0;
        reader.read_exact(&mut name).unwrap();
        reader.read_exact(&mut clan).unwrap();
        let grade = reader.read_i32::<LittleEndian>().unwrap();
        let contribution = reader.read_u16::<LittleEndian>().unwrap();
        let index = reader.read_u8().unwrap();
        let level = reader.read_u16::<LittleEndian>().unwrap();

        LegacyCharacterInfo {
            pov,
            name,
            clan,
            grade,
            contribution,
            index,
            level,
            will_do_the_rest_later: [0; 0x16f],
        }
    }
}

impl fmt::Display for LegacyCharacterInfo {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let (name, _, _) = WINDOWS_1252.decode(&self.name);
        let (clan, _, _) = WINDOWS_1252.decode(&self.clan);

        write!(
            f,
            "Name: {}; clan: {}; level: {}",
            name.trim_matches(char::from(0)),
            clan.trim_matches(char::from(0)),
            unsafe { &self.level }
        )
    }
}

fn main() {
    let args: Vec<String> = args().collect();

    if args.len() != 3 {
        panic!("Invalid number of arguments.");
    }

    let directory = Path::new(&args[1]);
    println!("Path: {}", directory.display());
    println!();

    let player_name = String::from(&args[2]);

    let mut matches = 0;
    let mut scanned = 0;
    let mut hit = false;

    for path in directory.read_dir().unwrap() {
        let file = path.unwrap().path();

        if file.extension().and_then(OsStr::to_str) == Some("gzr") {
            let contents: Vec<u8> = match read(&file) {
                Ok(result) => result,
                Err(_) => continue,
            };

            scanned += 1;

            let setting = LegacyStageSetting::new(&contents, 8);

            let offset: usize = if setting.game_type == 10 {
                8 * 16 + 3
            } else {
                0
            };

            let players = Cursor::new(&contents[0x4c + offset..0x50 + offset])
                .read_i32::<LittleEndian>()
                .unwrap();

            for player in 0..players {
                let chara = LegacyCharacterInfo::new(
                    &contents,
                    0x50 + offset + player as usize * size_of::<LegacyCharacterInfo>(),
                );

                let (name, _, _) = WINDOWS_1252.decode(&chara.name);
                let name = name.trim_matches(char::from(0));

                if name == player_name {
                    if !hit {
                        hit = true;
                        println!("Replays featuring {}:", player_name);
                        println!();
                    }

                    println!("    {:?}", file.as_os_str());

                    matches += 1;
                }
            }
        }
    }

    if hit {
        println!();
        print!(
            "{} match{} found,",
            matches,
            if matches == 1 { "" } else { "es" }
        );
    } else {
        print!("No matches found,");
    }

    println!(
        " {} valid replay{} scanned.",
        scanned,
        if scanned == 1 { "" } else { "s" }
    );
}
