# 2048!

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

![compile](https://github.com/dmfrodrigues/2048/workflows/compile/badge.svg)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/7a5830bdcc7a4ee9b9bd3555b92aaead)](https://www.codacy.com/gh/dmfrodrigues/2048/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=dmfrodrigues/2048&amp;utm_campaign=Badge_Grade)

- **Project name:** 2048!
- **Short description:** Clone of [2048](https://github.com/gabrielecirulli/2048)
- **Environment:** Unix/Windows
- **Tools:** C++, [SFML](https://www.sfml-dev.org/)
- **Institution:** [AECM](https://aecastelomaia.pt/)
- **Course:** API B (Informatic Applications B)

This project uses [SFML](https://www.sfml-dev.org/), a simple C++ graphics library. Further installation tutorials are available [here](https://www.sfml-dev.org/tutorials/2.5/). If you are under Linux, the easiest way is to check if it is available in your distribution's package repository (e.g. for Debian, `sudo apt install libsfml-dev`)

## Installing

Under Microsoft Windows, it is likely that, even if you add the SFML `include` and `lib` folders to PATH, `g++` will still ignore those. Instead, you can run `make` (or `mingw32-make`) and specify the paths where SFML headers and libraries are, by running for instance:
```cmd
make SFML_INCLUDE="C:\sfml\SFML-2.5.1\include" SFML_LIB="C:\sfml\SFML-2.5.1\lib"
```

## Third-party resources

`brittanic-bold-regular.ttf` was obtained for free from [FontsGeek](https://fontsgeek.com/fonts/Britannic-Bold-Regular).

`newScore_sound_v01.wav` was obtained from an open-source stock sound library.

`TheoTown_Song.ogg` is an excerpt from [戦うか逃げますか](https://www.youtube.com/watch?v=D2hi0Vzb2fw), an original composition by [Lucas King](https://www.youtube.com/user/LucasKingPiano/featured). This song was used under the *fair use for education*. I discovered this song through [TheoTown](https://hi.theotown.com/).

## License

© 2018-2020 Diogo Rodrigues

All files authored by me are licensed under [GNU General Public License v3](LICENSE) by **© 2018-2020 Diogo Rodrigues**.
