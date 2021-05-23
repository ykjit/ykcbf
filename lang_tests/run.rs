use std::{fs::read_to_string, path::PathBuf, process::Command};

use lang_tester::LangTester;
use lazy_static::lazy_static;
use regex::{Regex, RegexBuilder};

lazy_static! {
    static ref EXPECTED: Regex = RegexBuilder::new("^\\[(.*?)^\\][ \t]*$")
        .multi_line(true)
        .dot_matches_new_line(true)
        .build()
        .unwrap();
}

fn main() {
    LangTester::new()
        .test_dir(".")
        .test_file_filter(|p| {
            p.extension().map(|x| x.to_str().unwrap()) == Some("bf")
        })
        .test_extract(|p| {
            EXPECTED
                .captures(&read_to_string(p).unwrap())
                .map(|x| x.get(1).unwrap().as_str().trim().to_owned())
                .unwrap()
        })
        .test_cmds(|p| {
            let bf_bin = PathBuf::from("../bf");
            let mut bf = Command::new(bf_bin);
            bf.args(&[p]);
            vec![("bf", bf)]
        })
        .run();
}
