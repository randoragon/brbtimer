# brbtimer - a simple livestreaming timer

## WORK IN PROGRESS

THIS SOFTWARE IS NOT FINISHED. DO NOT ATTEMPT ANY FORM OF INSTALLATION YET.  
YOU HAVE BEEN WARNED.

---

This is a tiny utility program that allows setting a timer in seconds,
and then visualizes the passage of time with a simple animation sequence.

Its intended use is for livestreaming, in screencasting software like OBS
you can create a window capture and use chroma keys to isolate the animation
from the contrasting background to have a nice overlay for your recording scene.

I never intended to share this software, because I created it for myself,
but there's no harm sharing it in an open-source manner, is there?


## Building from source (GNU/Linux)

### Prerequisites

- GNU GCC compiler
- [Allegro 5](https://liballeg.org/)

### Installation

```sh
git clone https://github.com/randoragon/brbtimer
cd brbtimer
sudo make install
```

## Other Platforms

brbtimer was built using the [Allegro 5](https://liballeg.org/) library, which is cross-platform.
That said, I've never tried compiling for other operating systems,
so if you're a Windows, Mac or other user, the first step would probably be looking
inside the Makefile and figuring out how to run the appropriate commands on your system.
