# brbtimer - a simple livestreaming timer

This is a tiny utility program that allows setting a timer in seconds,
and then visualizes the passage of time with a simple animation sequence.

Its intended use is for livestreaming, in screencasting software like OBS
you can create a window capture and use chroma keys to isolate the animation
from the contrasting background to have a nice overlay for your recording scene.

I never intended to share this software, because I created it for myself,
but there's no harm sharing it in an open-source manner, is there?

## Usage

brbtimer needs to be invoked from a command line interface in order to work.
You pass the amount of time you want it to count down via command parameters.  
The quickest way to understand the syntax is with examples:

```sh
brbtimer 10s    # sets timer to 10 seconds
brbtimer 3m     # sets timer to 3 minutes
brbtimer 5h     # sets timer to 5 hours
```

You can pass as many options as you like, the sum will be calculated:

```sh
brbtimer 1s 2s 3s 4s   # sets timer to 10 seconds
brbtimer 1h 5m 10      # sets timer to 1 hour, 5 minutes and 10 seconds
brbtimer 2h 3m 1h 4m   # sets timer to 3 hours, 7 minutes
```

The default time unit is seconds, so 's' can be omitted:

```sh
brbtimer 5      # sets timer to 5 seconds
brbtimer 1m 2s  # sets timer to 1 minute, 2 seconds
```

## Building from source (GNU/Linux)

### Prerequisites

- GNU GCC compiler
- [Allegro 5](https://liballeg.org/)

### Installation

```sh
git clone https://github.com/randoragon/brbtimer
cd brbtimer
make
```

## Other Platforms

brbtimer was built using the [Allegro 5](https://liballeg.org/) library, which is cross-platform.
That said, I've never tried compiling for other operating systems,
so if you're a Windows, Mac or other user, the first step would probably be looking
inside the Makefile and figuring out how to run the appropriate commands on your system.
