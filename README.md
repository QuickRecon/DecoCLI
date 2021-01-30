# DecoCLI
### DO NOT USE THIS FOR ANY ACTUAL DIVE CALCULATIONS!

## Usage

This program is designed to perform (diving) decompression calculations.

General use: DecoCLI \<Mode> \<General Parameters> \<Dive Parameters>

### Modes:
BM             :   Use the buhlmann decompression model.

GF             :   Use the buhlmann decompression model with gradient factor support. (default is 30,80)

### General Parameters:
--ShowLicense  :   Show the extended license prompt.

--HideLicense  :   Hide the automatic license prompt.

--quiet        :   Suppress unnecessary outputs.

--help         :   Display this help message.

### Dive Parameters:
G              :   Adds a Gas to the Gas list, takes the form of G\<FrN2>:\<FrO2>:\<FrHe>,
                   air (expressed as G0.21:0.79:0) is automatically added as the zeroth element
                   and Gases can be accessed in the order they are added.

D              :   Add depths and times to the model, takes the form D\<Depth1>,\<Time1>:\<Depth2>,\<Time2>...
                   For example a dive that goes to 10 meters for 10 minutes then goes to 60 meters for 5 minutes
                   would be expressed as D10,10:60,5

## Development
### TODO
- [x] Implement buhlmann decompression model
- [x] Implement Gradient Factors in Deco Model
- [ ] Implement VPM Deco Model
- [x] Implement basic command line interface.
- [x] Implement custom GF values
- [ ] Allow for Gas switching
- [ ] Check models for accuracy against a known good program

## Sources

http://www.lizardland.co.uk/DIYDeco.html

http://www.ddplan.com/reference/mvalues.pdf

http://www.rebreatherworld.com/showthread.php?5037-Gradient-Factors-for-Dummies
