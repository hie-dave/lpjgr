# Debug script
# Note that this should be run from inside this directory (lpjgr/debug)
# Also note that the lpj-guess repo must be checked out at lpjgr/src/lpj-gues
# and that it should contain a file run/global_cf.ins which bootstraps
# the environment.

library('lpjgr')

# change to dave/run so we can use the data there
setwd('../src/lpj-guess/run')
insFile <- 'global_cf.ins'

# dummy met data
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

# call init()
initialise(insFile, lat, lon, co2, tmin, tmax, prec, insol, ndep, wind, relhum)

# # silly little function because why not
# get_vcmaxes <- function() {
#     vc_maxes <- c()
#     for (pft in get_pft_names()) {
#         vc_maxes[pft] <- get_spft_numeric_output(pft, 'vm')
#     }
#     return(vc_maxes)
# }

# get_vcmaxes()
canexch()
# get_vcmaxes()

pfts <- get_pft_names()
outputs <- list_spft_numeric_outputs()

for (output in outputs) {
    print(get_output_info(output))
}

data <- matrix(nrow = length(pfts), ncol = length(outputs))
rownames(data) <- pfts
colnames(data) <- outputs

for (output_name in outputs) {
    for (pft in pfts) {
        data[pft, output_name] <- get_spft_numeric_output(pft, output_name)
    }
}

data
