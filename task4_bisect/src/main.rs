use std::{env, fs, path::PathBuf, process::exit, process::Command, str};

fn check_path(path_str: &str) -> Result<PathBuf, ()> {
    let paths = fs::read_dir(path_str).unwrap_or_else(|_| exit(1));
    for path in paths {
        let name = path.as_ref().unwrap().file_name().into_string().unwrap();
        if name == ".git" {
            return Ok(PathBuf::from(path_str));
        }
    }
    Err(())
}

fn get_commits(path: &PathBuf) -> Vec<String> {
    let git_log = Command::new("git")
        .current_dir(path)
        .arg("log")
        .arg("--pretty=format:\"%h\"")
        .output()
        .expect("Oopsie");
    let hashes: Vec<String> = str::from_utf8(&git_log.stdout)
        .unwrap()
        .split('\n')
        .map(|mut x| {
            x = x.strip_prefix("\"").unwrap();
            x = x.strip_suffix("\"").unwrap();
            x.to_string()
        })
        .collect();
    println!("{:?}", hashes);
    hashes
}

fn checkout_commit(hash: &str, path: &PathBuf) {
    let _ = Command::new("git")
        .arg("switch")
        .arg("--detach")
        .current_dir(path)
        .arg(hash)
        .status()
        .expect("unlucky");
}

fn get_commit_range(commits: Vec<String>, first_commit: &str, second_commit: &str) -> Vec<String> {
    let mut delete_or_not_delete = false;
    let mut new_vec = Vec::new();
    for val in commits.iter() {
        if val == first_commit || val == second_commit {
            delete_or_not_delete = !delete_or_not_delete;
        }
        if delete_or_not_delete {
            new_vec.push(val.to_owned());
        }
    }
    if new_vec.contains(&first_commit.to_string()) {
        new_vec.push(second_commit.to_string());
    } else {
        new_vec.push(first_commit.to_string());
    }
    new_vec
}

fn check_commit(path: &PathBuf, hash: &str, command: &str) -> bool {
    checkout_commit(hash, path);
    let status = Command::new(command).status().expect("Ooops");
    status.success()
}

fn bisect(path: &PathBuf, range: Vec<String>, command: &str) -> String {
    let mut left = 0;
    let mut right = range.len() - 1;
    let mut pivot = 0;
    while left < right {
        pivot = (left + right) / 2;
        if check_commit(path, &range[pivot], command) {
            right = pivot - 1;
        } else if !check_commit(path, &range[pivot], command) {
            left = pivot + 1;
        }
    }
    range[pivot].to_owned()
}

fn main() {
    let args: Vec<_> = env::args().collect();
    match check_path(&args[1]) {
        Err(_) => {
            println!("Seems like the path you gave to the script is wrong!");
            exit(1);
        }
        Ok(path) => {
            let path = path.canonicalize().unwrap();
            let full_vec = get_commits(&path);
            let range = get_commit_range(full_vec.clone(), &args[2], &args[3]);
            println!("{}", bisect(&path, range, &args[4]));
            checkout_commit(&full_vec[0], &path);
        }
    }
}
