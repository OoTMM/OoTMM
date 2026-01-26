<!-- HEADER -->
<p align="center">
  <a id="readme-top"></a>
  <a href="https://ootmm.com/">
    <img alt="OoTMM Logo" with="256" height="256" src="./packages/gui/src/assets/logo-banner-rounded.png"/>
  </a>
  <p align="center">
    A randomizer of <em>The Legend of Zelda: Ocarina of Time</em> and <em>The Legend of Zelda: Majora's Mask</em> for the <em>Nintendo 64</em> that randomizes and combines both games into a single ROM.
  </p>
  <p align="center">
    <em>Created by: <a href="https://github.com/Nax">Maxime Bacoux "Nax"</a></em>
  </p>
</p>




<!-- BADGES -->
<p align="center">
    <img alt="badge-version" src="https://img.shields.io/github/v/tag/OoTMM/OoTMM?label=version&sort=semver" />
    <img alt="badge-license" src="https://img.shields.io/github/license/OoTMM/OoTMM" />
    <img alt="badge-build" src="https://img.shields.io/github/actions/workflow/status/OoTMM/OoTMM/ci.yml?branch=master" />
    <img alt="badge-stars" src="https://img.shields.io/github/stars/OoTMM/OoTMM" />
    <img alt="badge-discord" src="https://discordapp.com/api/guilds/1004394204992118935/widget.png?style=shield" />
</p>




<!-- QUICK LINKS -->
<p align="center">
  <strong>
  <a href="https://ootmm.com/gen/stable">Web Generator</a>
  &nbsp;&bull;&nbsp;
  <a href="./CHANGELOG.md">Changelog</a>
  &nbsp;&bull;&nbsp;
  <a href="https://ootmm.wiki.gg/">Wiki</a>
  &nbsp;&bull;&nbsp;
  <a href="https://discord.gg/4QdtPBP6wf">Discord</a>
  </strong>
</p>




<!-- TABLE OF CONTENTS -->
<p>
  <h2 id="table-of-contents">
    📑 Table of Contents
  </h2>
  <ul>
    <li>
      <a href="#usage">Usage</a>
    </li>
    <ul>
      <li>
        <a href="#rom-requirements">ROM Requirements</a>
      </li>
      <!-- <ul>
        <li>
          <a href="#ocarina-of-time">Ocarina of Time</a>
        </li>
        <li>
          <a href="#majoras-mask">Majora's Mask</a>
        </li>
      </ul> -->
      <li>
        <a href="#rom-byte-order">ROM Byte Order</a>
      </li>
    </ul>
    <li>
     <a href="#changelog">Changelog</a>
    </li>
    <li>
      <a href="#features">Features</a>
    </li>
    <li>
      <a href="#local-setup">Local Setup</a>
    </li>
    <ol>
      <li>
        <a href="#installing-the-required-tools">Installing the Required Tools</a>
      </li>
      <li>
        <a href="#download-or-clone-the-projects-repository">Download or Clone the Project's Repository</a>
      </li>
      <li>
        <a href="#adding-the-roms">Adding the ROMs</a>
      </li>
      <li>
        <a href="#environment-setup">Environment Setup</a>
      </li>
      <li>
        <a href="#building-a-rom">Building a ROM</a>
      </li>
    </ol>
    <li>
      <a href="#related-projects">Related Projects</a>
    </li>
  </ul>
</p>




<!-- USAGE -->
<h2 id="usage">
  ⚙️ Usage
</h2>

To generate an OoTMM Randomizer seed, use the [web generator][ootmm-randomizer-gen-stable]. For local generation, see <a href="#local-setup">Local Setup</a>

<p align="right"><sub><a href="#readme-top">Back to Top</a> 🔝</sub></p>

### ROM Requirements

OoTMM requires clean, unmodified, and internally compressed ROMs for *Ocarina of Time* and *Majora's Mask*. Using unsupported ROMs will prevent the randomizer from generating a randomized ROM.

The tables below list the OoTMM-compatible ROMs, along with their byte order, version, and MD5 checksum for verification.

#### Ocarina of Time

<table>
  <thead>
    <!-- <tr>
      <th colspan="5">Ocarina of Time</th>
    </tr> -->
    <tr>
      <th>Region</th>
      <th>Version</th>
      <th>Byte Order</th>
      <th>MD5 Checksum</th>
    </tr>
  </thead>
  <tbody>
    <!-- NTSC-U -->
    <tr align="center">
      <td>NTSC-U</td>
      <td>1.0</td>
      <td>Big-endian</td>
      <td>5BD1FE107BF8106B2AB6650ABECD54D6</td>
    </tr>
    <tr align="center">
      <td>NTSC-U</td>
      <td>1.0</td>
      <td>Little-endian</td>
      <td>AA3911F5D5598E19E0183E15B6719C36</td>
    </tr>
    <tr align="center">
      <td>NTSC-U</td>
      <td>1.0</td>
      <td>Byte-swapped</td>
      <td>6697768A7A7DF2DD27A692A2638EA90B</td>
    </tr>
    <!-- NTSC-J -->
    <tr align="center">
      <td>NTSC-J</td>
      <td>1.0</td>
      <td>Big-endian</td>
      <td>9F04C8E68534B870F707C247FA4B50FC</td>
    </tr>
    <tr align="center">
      <td>NTSC-J</td>
      <td>1.0</td>
      <td>Little-endian</td>
      <td>ECEF4DD132C4398A6A69F90038303E0A</td>
    </tr>
    <tr align="center">
      <td>NTSC-J</td>
      <td>1.0</td>
      <td>Byte-swapped</td>
      <td>7D44B555E0AF3EEC36319B5E76E31B0C</td>
    </tr>
  </tbody>
</table>

#### Majora's Mask

<table>
  <thead>
    <!-- <tr>
      <th colspan="5">Majora's Mask</th>
    </tr> -->
    <tr>
      <th>Region</th>
      <th>Version</th>
      <th>Byte Order</th>
      <th>MD5 Checksum</th>
    </tr>
  </thead>
  <tbody>
    <!-- NTSC-U -->
    <tr align="center">
      <td>NTSC-U</td>
      <td>1.0</td>
      <td>Big-endian</td>
      <td>2A0A8ACB61538235BC1094D297FB6556</td>
    </tr>
    <tr align="center">
      <td>NTSC-U</td>
      <td>1.0</td>
      <td>Little-endian</td>
      <td>FC39208428BCAE70508E633451C11CC0</td>
    </tr>
    <tr align="center">
      <td>NTSC-U</td>
      <td>1.0</td>
      <td>Byte-swapped</td>
      <td>B0CC37A45BC1B6C76CD7F0B71FE2BB1F</td>
    </tr>
  </tbody>
</table>

> [!TIP]
> To verify your ROMs, calculate their MD5 checksums using <a href="https://emn178.github.io/online-tools/md5_checksum.html" target="_blank" title="MD5 Checksum Tool">this MD5 checksum tool</a>. The calculated checksums must match the values listed in the tables above.

<p align="right"><sub><a href="#readme-top">Back to Top</a> 🔝</sub></p>

### ROM Byte Order

OoTMM does not require ROMs to be in big-endian byte order. However, using ROMs in other byte orders can negatively impact seed generation performance, as the randomizer must correct the byte order internally. For best performance, it is recommended to use ROMs in big-endian byte order.

A ROM's byte order can often be inferred at a glance by its file extension:

- `📄.z64` — Typically big-endian.
- `📄.n64` — Typically little-endian.
- `📄.v64` — Typically byteswapped.

> [!WARNING]
> The file extension *does not* guarantee a ROM's byte order is correct. Renaming the file *does not* change its byte order. To correctly verify or convert a ROM's byte order, use <a href="https://www.zophar.net/download_file/2854" target="_blank" title="Click to Download Tool64">Tool64</a>.

> [!TIP]
> To convert a ROM to big-endian with Tool64, follow these steps:
> > 1. Open Tool64.
> > 2. Select **File > Open...** from the menu bar.
> > 3. Select the folder containing your ROM file.
> > 4. Click **OK**.
> > 5. In the main window, select your ROM file.
> > 6. Right-click the file and choose **Big Endian** from the context menu.

<p align="right"><sub><a href="#readme-top">Back to Top</a> 🔝</sub></p>




<!-- CHANGELOG -->
<h2 id="changelog">
  📝 Changelog
</h2>

For a list of changes between stable release versions, see [CHANGELOG.md][changelog].

<p align="right"><sub><a href="#readme-top">Back to Top</a> 🔝</sub></p>



<!-- SUPPORT -->
<h2 id="support">
  🧩 Support
</h2>

To get help with the randomizer or your playthrough, join the community's [Discord server][ootmm-discord] and ask in one of the following channels:

- `#tech-support` — Help with emulators or other technical issues.
- `#playthrough-help` — Assistance with an OoTMM playthrough.
- `#settings-help` — For questions about specific randomizer settings.
- `#tracker-support` — Support for tracker-related issues.

<p align="right"><sub><a href="#readme-top">Back to Top</a> 🔝</sub></p>




<!-- FEATURES -->
<h2 id="features">
  ✨ Features
</h2>

Along with randomizing items across both games, the randomizer includes several additional features to enhance your experience.

<!--
    Potential features to add/list:
    -
-->

### Custom Music

Beyond randomizing the original songs, OoTMM supports adding custom music not included in *The Legend of Zelda: Ocarina of Time* and *The Legend of Zelda: Majora's Mask*.

#### Adding Music to OoTMM

To use custom music with the web generator, follow these steps:

> 1. Compress your music files into a ZIP file.
> 2. In the generator, select **Cosmetics**.
> 3. On the cosmetics page, click the **Custom Music ZIP** box.
> 4. Select the ZIP file containing your music files.

> [!TIP]
> In Windows 11, Windows Explorer can compress files without additional software. To compress files into a ZIP file, follow these steps:
> > 1. Select the files you want to compress.
> > 2. Right-click the files and select **Compress to... > ZIP File**.

#### Feature Support

<table>
  <thead>
    <tr>
      <th>Supported</th>
      <th>File or Feature</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td align="center">✅</td>
      <td>Ocarina of Time Randomizer custom music files (<code>🗃️.ootrs</code>)</td>
    </tr>
    <tr>
      <td align="center">✅</td>
      <td>Majora's Mask Randomizer custom music files (<code>🗃️.mmrs</code>)</td>
    </tr>
    <tr>
      <td align="center">✅</td>
      <td>Custom audio sequences</td>
    </tr>
    <tr>
      <td align="center">✅</td>
      <td>Custom instrument banks</td>
    </tr>
    <tr>
      <td align="center">⛔</td>
      <td>Custom audio samples</td>
    </tr>
    <tr>
      <td align="center">⛔</td>
      <td>Formmmask</td>
    </tr>
  </tbody>
</table>

> [!WARNING]
> Files that do not work with the randomizer will show a warning and be skipped during seed generation.

#### Obtaining Custom Music

If you do not already have any custom music, you can download custom music files from any of the resources below:

- [OoTMM Discord Server][ootmm-discord] — Found in the `#music-releases` channel of the server.
- [Darunia's Joy Repository][darunias-joy] — A community resource of Ocarina of Time Randomizer custom music files.
- [MMR Discord Server][mmr-discord] — Found in the `#music-releases` channel of the server.
- [Majora's Music-Box House Repository][majoras-music-box-house] — A community resource of Majora's Mask Randomizer custom music files.

> [!IMPORTANT]
> For Majora's Mask Randomizer custom music files, standalone sequence files (`📄.zseq`) are not supported by OoTMM.


<p align="right"><sub><a href="#readme-top">Back to Top</a> 🔝</sub></p>



<!-- LOCAL SETUP -->
<h2 id="local-setup">
  🛠️ Local Setup
</h2>

OoTMM can be built and run locally using either a **Dev Container** (recommended) or your **native environment** by following the stepd detailed below.

<h3 id="installing-the-required-tools">
  1. Installing the Required Tools
</h3>

Before building, ensure the following tools are installed:

#### Dev Container

- [Docker][docker]
- [Visual Studio Code][visual-studio-code]
- [Dev Containers Extension][dev-containers-ext]

#### Native Build

- **C/C++ toolchain**:
    - [GCC][gcc] (Linux, macOS, or Windows via [MSYS2][msys2])
    - [CLang][clang] (macOS, Linux, or Windows via [LLVM][llvm])
    - [Visual C++ Build Tools][visual-cpp-build-tools] (Windows)
- [CMake][cmake]
- [Ninja][ninja-build]
- [n64-ultra][n64-ultra]
- [Node][node-js] (Version 22 or newer)
- [Git][git]

> [!TIP]
> On Windows, it is recommended to use [Windows Subsystem for Linux][wsl] for a better build experience.

<h3 id="download-or-clone-the-projects-repository">
  2. Download or Clone the Project's Repository
</h3>

#### Downloading Project from GitHub

To download the project from GitHub, follow these steps:

> 1. Click **Code**.
> 2. Select **Download ZIP** from the menu.
> 3. Extract the contents of the ZIP file to a folder of your choice.

#### Cloning Project with Git

To clone the project with Git, follow these steps:

> 1. Open a terminal in the folder of your choice.
> 2. Run the following command:
> ```
> git clone https://github.com/OoTMM/OoTMM.git
> ```

<h3 id="adding-the-roms">
1. Adding the ROMs
</h3>

To prepare the ROMs for seed generation, follow these steps:

> 1. Rename your *Ocarina of Time* ROM to: `oot.z64`.
> 2. Rename your *Majora's Mask* ROM to: `mm.z64`.
> 3. Place both your ROMs into the `📁/roms` folder in the project's root directory:

<h3 id="environment-setup">
4. Environment Setup
</h3>

#### Dev Container

To set up the environment with Docker, follow these steps:

> 1. Open Visual Studio Code.
> 2. Click **File > Open Folder...** and select the project's folder.
> 3. When prompted, click **Reopen in Container**.

#### Native

To set up the environment natively, follow these steps:

> 1. Open a terminal in the project's root directory.
> 2. Run the setup script:
> ```
> ./setup.sh
> ```

<h3 id="building-a-rom">
1. Building a ROM
</h3>

Once the setup is complete, you can create a randomized ROM using the following command:
```
pnpm start:core:config
```

> [!NOTE]
> **ROM Output Location**
>
> The randomized ROM will be located in the `📁/out` folder.

<p align="right"><sub><a href="#readme-top">Back to Top</a> 🔝</sub></p>




<!-- RELATED PROJECTS -->
<h2 id="related-projects">
  🔗 Related Projects
</h2>

- **Ocarina of Time Randomizer**
    - [Website][oot-randomizer-site]
    - [Discord][ootr-discord]
    - [GitHub][oot-randomizer-github]
- **Majora's Mask Randomizer**
    - [Website][mm-randomizer-site]
    - [Discord][mmr-discord]
    - [GitHub][mm-randomizer-github]

<p align="right"><sub><a href="#readme-top">Back to Top</a> 🔝</sub></p>




<!-- LINKS -->
<!--
    Links here use reference style linking:
    https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[docker]: https://www.docker.com/
[visual-studio-code]: https://code.visualstudio.com/
[dev-containers-ext]: https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers
[gcc]: https://gcc.gnu.org/
[msys2]: https://www.msys2.org/
[clang]: https://clang.llvm.org/
[llvm]: https://releases.llvm.org/download.html
[visual-cpp-build-tools]: https://visualstudio.microsoft.com/visual-cpp-build-tools/
[cmake]: https://cmake.org/
[ninja-build]: https://ninja-build.org/
[node-js]: https://nodejs.org/en
[git]: https://git-scm.com/
[n64-ultra]: https://github.com/glankk/n64
[wsl]: https://learn.microsoft.com/windows/wsl/install


<!-- Local Repo Files -->
[license]: LICENSE
[readme]: README.md
[changelog]: CHANGELOG.md

<!-- OOTMM -->
[ootmm-randomizer-site]: https://ootmm.com
[ootmm-randomizer-gen-stable]: https://ootmm.com/gen/stable
[ootmm-randomizer-gen-dev]: https://ootmm.com/gen/dev
[ootmm-randomizer-wiki]: https://ootmm.wiki.gg/

<!-- Related Projects -->
[oot-randomizer-site]: https://ootrandomizer.com/
[oot-randomizer-github]: github.com/OoTRandomizer/OoT-Randomizer
[mm-randomizer-site]: https://mmrandomizer.com/
[mm-randomizer-github]: https://github.com/ZoeyZolotova/mm-rando

<!-- Social Links -->
[ootmm-discord]: https://discord.gg/4QdtPBP6wf
[ootr-discord]: https://discord.gg/ootrandomizer
[mmr-discord]: https://discord.gg/8qbreUM

<!-- Custom Music Link -->
[darunias-joy]: https://github.com/DaruniasJoy/OoT-Custom-Sequences
[majoras-music-box-house]: https://github.com/MajorasMusicBoxHouse/Majoras-MusicBox-House

<!-- Contributors -->
[user-nax]: https://github.com/Nax
[user-celestialkitsune]: https://github.com/CelestialKitsune
[user-xenowars]: https://github.com/XenoWars
[user-zoeyzolotova]: https://github.com/ZoeyZolotova
[user-joshua8600]: https://github.com/Joshua8600
[user-amazingampharos]: https://github.com/AmazingAmpharos
[user-revvenn]: https://github.com/Revvenn
[user-matthewkirby]: https://github.com/matthewkirby
[user-ghunzor]: https://github.com/Ghunzor
[user-lkarvec]: https://github.com/lkarvec
[user-aegiker]: https://github.com/Aegiker
[user-eedefeed]: https://github.com/eedefeed
[user-hamelatoire]: https://github.com/Hamelatoire
[user-ggkfox]: https://github.com/ggkfox
[user-zeemaji]: https://github.com/ZeeMaji
[user-lumineonrl]: https://github.com/LumineonRL
[user-sciencenerd2240]: https://github.com/ScienceNerd2240
[user-skyros4]: https://github.com/Skyros4
[user-itsbirdseed]: https://github.com/itsbirdseed
[user-demitastes]: https://github.com/demitastes
[user-matthe815]: https://github.com/matthe815
[user-jupiter0fire]: https://github.com/jupiter0fire
[user-kethku]: https://github.com/Kethku
[user-crinuleiroz]: https://github.com/crinuleiroz
