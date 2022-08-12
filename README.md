# LPJ-Guess R Wrapper

R wrapper around certain LPJ-Guess subroutines.

## Getting Started

You'll need to make the lpj-guess repository accessible at src/lpj-guess. I'd recommend a symlink, but a full checkout here should also work.

```bash
cd /path/to/lpjgr
ln -s /path/to/lpj-guess src/lpj-guess
# OR
# cd /path/to/lpjgr/src
# svn checkout <repo url> lpj-guess
```

## Building

```bash
R CMD build
R CMD check
```

## Development

Open the project file in RStudio and click install.

## Usage

```R
# remotes::install_github('hol430/lpjgr')
library('lpjgr')
lambert_beer(-2)
```
