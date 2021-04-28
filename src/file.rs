use std::fs::File;
use std::io::Error;
use std::io::Read;
use std::path::Path;

use flate2::bufread::ZlibDecoder;

pub fn read<P: AsRef<Path>>(path: P) -> Result<Vec<u8>, Error> {
    let mut file = File::open(&path).unwrap();
    let mut vector: Vec<u8> = Vec::new();

    file.read_to_end(&mut vector).unwrap();
    decode(vector)
}

fn decode(bytes: Vec<u8>) -> Result<Vec<u8>, Error> {
    let mut decoder = ZlibDecoder::new(&bytes[..]);
    let mut output = Vec::new();

    match decoder.read_to_end(&mut output) {
        Ok(_) => Ok(output),
        Err(why) => Err(why),
    }
}
