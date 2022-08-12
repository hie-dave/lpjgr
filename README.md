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

- Requires `libguess.so` may be resolved by the dynamic loader. You can check if this is the case by running `ldconfig -p | grep guess`. Should be 1 row of output showing the location of the library. Note that trunk lpj-guess does not build a shared object library - talk to Drew to get this working.

## Development

Open the project file in RStudio and click install (given the above caveat re building guess as a shared lib).

## Installation

This is not necessary if following the above steps in the Development section.

```R
# install.packages('remotes')
remotes::install_github('hol430/lpjgr')
```

## Usage

```R
library('lpjgr')
lambert_beer(-2) # Should print e
```

**Initialisation of the Environment**

It's important to call `initialise()` before most other functions (`lambert_beer()` is an exception here, it's a simple testing function with no external dependencies). `initialise()` will parse the .ins file, initialise climate drivers, landcover, etc, and will simulate the entire day up until the call to `canopy_exchange()` (this last bit will change in a future version). Note that `initialise()` will /not/ read any .met data from files, nor will it perform any spinup. The daily met data is all given as arguments to `initialise()`:

```R
insFile <- 'global.ins'
lat <- -27
lon <- 151
co2 <- 360
tmin <- 20
tmax <- 30
prec <- 0
insol <- 0.1
ndep <- 0.000225 / 365
wind <- 12
relhum <- 0.5
initialise(insFile, lat, lon, co2, tmin, tmax, prec, insol, ndep, wind, relhum)
```

Once this is done, we can begin running and querying things:

```R
# Print all PFT Names
> get_pft_names()
 [1] "BNE"   "BINE"  "BNS"   "TeNE"  "TeBS"  "IBS"   "TeBE"  "TrBE"  "TrIBE" "TrBR"  "C3G"   "C4G"
# Get vc_max for each PFT
> get_vcmaxes <- function() {
+     vc_maxes <- c()
+     for (pft in get_pft_names()) {
+         vc_maxes[pft] <- get_pft_property(pft, 'vm')
+     }
+     return(vc_maxes)
+ }
# All 0, because canopy_exchange has not been called yet
> get_vcmaxes()
  BNE  BINE   BNS  TeNE  TeBS   IBS  TeBE  TrBE TrIBE  TrBR   C3G   C4G 
    0     0     0     0     0     0     0     0     0     0     0     0 
# Run canopy exchange
> canexch()
# Now get vc_max for each PFT again
> get_vcmaxes()
     BNE     BINE      BNS     TeNE     TeBS      IBS     TeBE     TrBE    TrIBE     TrBR      C3G      C4G 
94.49396 94.49396 94.49396 94.48977 94.48977 94.49396 94.48977 94.12202 94.12202 94.12202 95.24241 44.49838 
```

## todo

- Rationalise output handling (search for outputs by data type)
- `initialise()` currently simulates the 1st day up to just before the call to `canopy_exchange()`. Make this customisable, so that the user can run it up to a given point in the day, in order to experiment with other functions(?)
