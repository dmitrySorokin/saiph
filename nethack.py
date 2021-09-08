import gym
import nle
from termcolor import colored
from nle.scripts import play
from nle.scripts.play import main
import time
from nle.nethack import Nethack


TTY_BRIGHT = 8


def get_tty_rendering(tty_chars, tty_colors):
    # TODO: Merge with "full" rendering below. Why do we have both?
    rows, cols = tty_chars.shape
    result = ""
    for i in range(rows):
        for j in range(cols):
            result += "\x1b[%d;3%dm%s" % (
                bool(tty_colors[i, j] & TTY_BRIGHT),
                tty_colors[i, j] & ~TTY_BRIGHT,
                chr(tty_chars[i, j]),
            )
        result += '\n'
    return result


def color_render(obs):
    tty_chars = obs["tty_chars"]
    tty_colors = obs["tty_colors"]
    rendering = get_tty_rendering(tty_chars, tty_colors)
    print(rendering)



class _Getch:
    """Gets a single character from standard input.  Does not echo to the
screen."""
    def __init__(self):
        try:
            self.impl = _GetchWindows()
        except ImportError:
            self.impl = _GetchUnix()

    def __call__(self): return self.impl()


class _GetchUnix:
    def __init__(self):
        import tty, sys

    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch


class _GetchWindows:
    def __init__(self):
        import msvcrt

    def __call__(self):
        import msvcrt
        return msvcrt.getch()


getch = _Getch()


if __name__ == '__main__':
    env = gym.make("NetHackChallenge-v0", savedir='replays')
    obs = env.reset()
    color_render(obs)

    chr2id = {
        chr(act.value): i for i, act in enumerate(env._actions)
    }
    #print(chr2id)
    #exit(0)
    log = open('log.txt', 'a')

    done = False
    while not done:
        action = getch()
        if action == '\n':
            action = '\r'
        log.write(f'action: "{action}", "{ord(action)}", "{chr2id.get(action)}"\n')
        log.flush()
        obs, reward, done, info = env.step(chr2id[action])
        for row in obs['tty_chars']:
            for ch in row:
                log.write(f'{chr(ch)}')
            log.write('\n')
        log.flush()
        color_render(obs)

