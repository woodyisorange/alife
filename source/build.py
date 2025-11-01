import argparse
import os
import subprocess

output_path = "../binaries"
compiler = ["clang++"]
debug_compile_flags = ["-g", "-Og"]
release_compile_flags = ["-O3"]
extra_compile_flags = [] # loaded from compile_flags.txt
compile_arguments = ["-o", f"{output_path}/tactile.elf", "sdl3_platform.cpp"]
link_arguments = ["-lm", "-lSDL3"]

args_parser = argparse.ArgumentParser()
args_parser.add_argument("target", nargs='*')
args = args_parser.parse_args()
targets = args.target

if len(targets) == 0:
    targets = ["debug"]

with open("compile_flags.txt", "r") as compile_flags_file:
    extra_compile_flags = compile_flags_file.read().splitlines()

def run_build(compile_flags):
    os.makedirs(output_path, exist_ok=True)
    command = compiler + compile_flags + extra_compile_flags + compile_arguments + link_arguments
    print(f"Build command: '{' '.join(command)}'")
    subprocess.run(command)

for target in targets:
    match target:
        case "debug":
            run_build(debug_compile_flags)
        case "release":
            run_build(release_compile_flags)
        case _:
            print(f"Unrecognised Target: '{target}'")

