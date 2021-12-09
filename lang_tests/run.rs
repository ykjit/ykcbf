use std::{fs::read_to_string, path::PathBuf, process::Command};

use lang_tester::LangTester;
use lazy_static::lazy_static;
use regex::{Regex, RegexBuilder};
use std::collections::HashMap;

lazy_static! {
    static ref EXPECTED: Regex = RegexBuilder::new("^\\[(.*?)^\\][ \t]*$")
        .multi_line(true)
        .dot_matches_new_line(true)
        .build()
        .unwrap();

    // Maps binary name to a list of benchmarks to skip.
    static ref BF_BINARIES: HashMap<&'static str, Vec<&'static str>> = HashMap::from([
        ("bf_base", vec![]),
        ("bf_simple_yk", vec!["hanoi.bf", "hello.bf", "numwarp.bf", "interp.bf", "bench.bf"]),
        ("bf_simple2_yk", vec!["hanoi.bf", "hello.bf", "numwarp.bf", "interp.bf", "bench.bf"]),
    ]);
}

fn main() {
    for (bf_bin, skips) in &*BF_BINARIES {
        eprintln!("Running tests with interpreter binary: {}", bf_bin);
        LangTester::new()
            .test_dir(".")
            .test_file_filter(|p| p.extension().map(|x| x.to_str().unwrap()) == Some("bf"))
            .test_extract(move |p| {
                if !skips.contains(&p.file_name().unwrap().to_str().unwrap()) {
                    EXPECTED
                        .captures(&read_to_string(p).unwrap())
                        .map(|x| x.get(1).unwrap().as_str().trim().to_owned())
                        .unwrap()
                } else {
                    "ignore: ignore".to_string()
                }
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
