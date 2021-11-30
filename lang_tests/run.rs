use std::{fs::read_to_string, path::PathBuf, process::Command};

use lang_tester::LangTester;
use lazy_static::lazy_static;
use regex::{Regex, RegexBuilder};

// FIXME: doesn't yet test the JIT.
static BF_BINARIES: &[&str] = &["bf_base"];

lazy_static! {
    static ref EXPECTED: Regex = RegexBuilder::new("^\\[(.*?)^\\][ \t]*$")
        .multi_line(true)
        .dot_matches_new_line(true)
        .build()
        .unwrap();
}

fn main() {
    for bf_bin in BF_BINARIES {
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
            .test_cmds(move |p| {
                let mut bf_path = PathBuf::from("..");
                bf_path.push(bf_bin);
                let mut bf = Command::new(bf_path);
                bf.args(&[p]);
                vec![("bf", bf)]
            })
            .run();
    }
}
