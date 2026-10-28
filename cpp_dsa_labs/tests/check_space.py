"""Check DSA_space using isolated input and output files."""

from pathlib import Path
import hashlib
import json
import subprocess

ROOT = Path(__file__).resolve().parents[1]
checks = 0


def require(value, message):
    global checks
    checks += 1
    if not value:
        raise AssertionError(message)


def scenario(config, name, commands, files=None, output_directory=False):
    folder = ROOT / 'build' / 'space-scenarios' / config / name
    folder.mkdir(parents=True, exist_ok=True)
    for filename, text in (files or {}).items():
        (folder / filename).write_bytes(text.encode('utf-8'))
    if output_directory:
        (folder / 'output.txt').mkdir(exist_ok=True)
    result = subprocess.run(
        [str(ROOT / 'build' / 'x64' / config / 'DSA_space' / 'DSA_space.exe')],
        input=commands.encode('utf-8'), cwd=folder,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=15,
    )
    require(result.returncode == 0, (config, name, result.stderr))
    return result.stdout.decode('utf-8'), result.stderr.decode('utf-8'), folder


working = [ROOT / 'lab_01' / 'input.txt', ROOT / 'lab_01' / 'output.txt',
           ROOT / 'homework' / 'data.txt']
before = {str(p): hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else None
          for p in working}

for config in ['Debug', 'Release']:
    cases = [('', ''), ('   \n\t ', ''), ('a bb\nccc aa', 'ccc\n'),
             ('aaaa\n  bbbb   aa\ncccc', 'aaaa\nbbbb\ncccc\n'),
             ('кот cats\nслон dogs', 'cats\nслон\ndogs\n'),
             ('\ufeff  aa bbb aa', 'bbb\n'),
             ('a ' + 'x' * 20000, 'x' * 20000 + '\n')]
    for index, (text, expected) in enumerate(cases):
        out, err, folder = scenario(config, f'words-{index}', '2\nwords.txt\n0\n',
                                    {'words.txt': text})
        require(not err, err)
        require((folder / 'output.txt').read_text(encoding='utf-8') == expected,
                (config, index))

    lines = ['A + (B * [C - {D}])', '([)]', 'no brackets', '(()', '}', '',
             '() [] {}', '(' * 1000 + ')' * 1000]
    expected = [True, False, True, False, False, True, True, True]
    for choice in [3, 4]:
        out, err, folder = scenario(config, f'brackets-{choice}',
                                    f'{choice}\nbrackets.txt\n0\n',
                                    {'brackets.txt': '\n'.join(lines) + '\n'})
        require(not err, err)
        for line, valid in zip(lines, expected):
            require(line + ' -> ' + ('Yes' if valid else 'No') in out,
                    (config, choice, line[:40]))
        require(not (folder / 'output.txt').exists(), 'Brackets must not write output.txt')

    out, err, folder = scenario(config, 'old-task', '1\n\n0\n',
                                {'input.txt': '8 3 2 7 1 6 5 4 -3 -5 -2'})
    require((folder / 'output.txt').read_text().split()
            == '-2 1 2 3 4 5 6 7 8 -5 -3'.split(), 'Old list task changed')

    out, err, folder = scenario(config, 'bad-number', '1\n\n0\n',
                                {'input.txt': '1 2 wrong', 'output.txt': 'keep'})
    require('Error:' in err, 'Invalid number was not reported')
    require((folder / 'output.txt').read_text() == 'keep', 'Output changed after error')

    invalid = ['', ' ', 'abc', '00', '0' * 30, '9' * 10000, '-1', '+2', '2abc', '1.5', '5']
    out, err, folder = scenario(config, 'bad-menu', '\n'.join(invalid) + '\n0\n')
    require(out.count('Enter an integer from 0 to 4:') == len(invalid), 'Wrong menu retry')
    require(not err, err)

    out, err, folder = scenario(config, 'retry-file', '2\nmissing.txt\n.\nwords.txt\n0\n',
                                {'words.txt': 'a bbb aa'})
    require(out.count('Enter another file:') == 2, 'File error returned to menu')
    require((folder / 'output.txt').read_text() == 'bbb\n', 'File retry failed')

    out, err, folder = scenario(config, 'same-file', '2\noutput.txt\nwords.txt\n0\n',
                                {'output.txt': 'old data', 'words.txt': 'a bb'})
    require('Input must differ' in out, 'Same-file input accepted')
    require((folder / 'output.txt').read_text() == 'bb\n', 'Retry after same-file error failed')

    out, err, folder = scenario(config, 'output-error', '2\nwords.txt\n0\n',
                                {'words.txt': 'a bb'}, output_directory=True)
    require('Cannot open output.txt' in err, 'Output error was not reported')
    require(out.count('0. Exit') >= 2, 'Output error stopped the menu')

    for name, commands in [('menu-eof', ''), ('file-eof', '2\n')]:
        scenario(config, name, commands)

    out, err, folder = scenario(config, 'repeat', '2\nwords.txt\n3\nbrackets.txt\n4\nbrackets.txt\n0\n',
                                {'words.txt': 'aaa b cc', 'brackets.txt': '([)]\n()\n'})
    require(out.count('([)] -> No') == 2 and out.count('() -> Yes') == 2,
            'Repeated operations reused stale state')

for p in working:
    after = hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else None
    require(after == before[str(p)], 'Working data changed: ' + str(p))

print(f'Passed {checks} scenario checks in Debug and Release; working data unchanged.')
