import json


def parse_line(line):
    splits = line.split()

    flags = []
    append_next = False
    prefix = ''
    for split in splits:
        if append_next:
            flags.append(prefix + split)
            append_next = False
            prefix = ''
        elif split.startswith('-I') or \
                split.startswith('-std='):
            flags.append(split)
        elif split == '-isystem':
            append_next = True
            prefix = '-isystem '
    return flags


def main():
    with open('compile_commands.json', 'r') as ccf:
        compile_commands = json.load(ccf)

    compile_flag_lines = [cc['command'].split(' ', 1)[1]
                          for cc in compile_commands]

    compile_flags = set()
    for line in compile_flag_lines:
        compile_flags.update(parse_line(line))
    compile_flags = list(compile_flags)

    with open('Doxyfile.clang', 'w') as df:
        if len(compile_flags) > 0:
            print('CLANG_ASSISTED_PARSING = YES', file=df)
            print('CLANG_OPTIONS = ' + compile_flags[0], end=' \\\n', file=df)
            if len(compile_flags) > 1:
                for cf in compile_flags[1:-1]:
                    print('                ' + cf, end=' \\\n', file=df)
                print('                ' + compile_flags[-1], file=df)

if __name__ == '__main__':
    print('Generating Doxyfile.clang.')
    main()
    print('Generated Doxyfile.clang.')
