/***************
 CTAG TBD >>to be determined<< is an open source eurorack synthesizer module.

 A project conceived within the Creative Technologies Arbeitsgruppe of
 Kiel University of Applied Sciences: https://www.creative-technologies.de

 (c) 2020 by Robert Manzke. All rights reserved.

 The CTAG TBD software is licensed under the GNU General Public License
 (GPL 3.0), available here: https://www.gnu.org/licenses/gpl-3.0.txt

 The CTAG TBD hardware design is released under the Creative Commons
 Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0).
 Details here: https://creativecommons.org/licenses/by-nc-sa/4.0/

 CTAG TBD is provided "as is" without any express or implied warranties.

 License and copyright details for specific submodules are included in their
 respective component folders / files if different from this license.
 ***************/


// this file is to parse the JSON mui files to generate the c++ code for parameter parsing
// a convenience effort towards a generator
// usage to create new files: node generator.js muifile
    // example node generator.js mui-SimpleVCA.jsn
// usage to overwrite / replace in existing files: node generator.js ProcessorName -i
    // example node generator.js SimpleVCA
    // attention this replaces code / overwrites the mp-...jsn file

const fs = require('fs');
let nargs = process.argv.length;
console.log("Have params: " + nargs);
if(nargs < 3){
    console.log('Usage, see source file!');
    process.exit(-1);
}

// process source files
let muiIn, mui, hppIn, hppOut, cppIn, cppOut, mp;
if(nargs == 3){
    muiIn = process.argv[2];
    mui = JSON.parse(fs.readFileSync(muiIn));
    hppIn = 'ctagSoundProcessorTemplate.hpp';
    hppOut = 'ctagSoundProcessor' + mui.id + '.hpp';
    cppIn = 'ctagSoundProcessorTemplate.cpp';
    cppOut = 'ctagSoundProcessor' + mui.id + '.cpp';
    mp = 'mp-' + mui.id + '.jsn';
}else if(nargs == 4){
    if(process.argv[3] != '-i'){
        console.log('Usage, see source file!');
        process.exit(-1);
    }
    muiIn = '../spiffs_image/data/sp/mui-' + process.argv[2] + '.jsn';
    mui = JSON.parse(fs.readFileSync(muiIn));
    mp = '../spiffs_image/data/sp/mp-' + process.argv[2] + '.jsn';
    hppIn = '../components/ctagSoundProcessor/ctagSoundProcessor' + process.argv[2] + '.hpp';
    hppOut = hppIn;
    cppIn = '../components/ctagSoundProcessor/ctagSoundProcessor' + process.argv[2] + '.cpp';
    cppOut = cppIn;
}else{
    console.log('Usage, see source file!');
    process.exit(-1);
}
// parse mui file
let sHeader = 'const string id = "' + mui.id + '";\n';
sHeader += 'void setIsStereo(){isStereo = ' + mui.isStereo + ';}\n';
let sCpp1 = '';
let sCpp2 = '';
let smp = {}; // preset object to be written as json
smp.activePatch = 0;
smp.patches = [];
smp.patches.push({"name": "Default", "params": []});
mui.params.forEach(el => parseData(el));
// create mp preset model
fs.writeFileSync(mp, JSON.stringify(smp, null, 2));
// generate header file
let shpp = fs.readFileSync(hppIn, 'utf8');
shpp = shpp.replace(/ctagSoundProcessorTemplate/g, 'ctagSoundProcessor' + mui.id);
let part = shpp.split('// sectionHpp');
part.splice(1, 1); // remove auto code section, to be replaced with new generation
let newHpp = part[0];
newHpp += '// sectionHpp\n';
newHpp += sHeader;
newHpp += '// sectionHpp\n';
newHpp += part[1];
fs.writeFileSync(hppOut, newHpp);
// generate source file
// part 1
let scpp = fs.readFileSync(cppIn, 'utf8');
scpp = scpp.replace(/ctagSoundProcessorTemplate/g, 'ctagSoundProcessor' + mui.id);
part = scpp.split('// sectionCpp0');
part.splice(1, 1);
let newCpp = part[0];
newCpp += '// sectionCpp0\n';
newCpp += sCpp1;
newCpp += '// sectionCpp0\n';
newCpp += part[1];
// part 2
part = newCpp.split('// sectionCpp1');
part.splice(1, 1);
newCpp = part[0];
newCpp += '// sectionCpp1\n';
newCpp += sCpp2;
newCpp += '// sectionCpp1\n';
newCpp += part[1];
fs.writeFileSync(cppOut, newCpp);



function parseData(el){
    switch(el.type){
        case 'group':
            el.params.forEach(v => parseData(v)); // recursive for group
            break;
        default:
            hppSource(el);
            cppSource1(el);
            cppSource2(el);
            smpSource(el);
            break;
    }
}

function hppSource(el){
    sHeader += "atomic<int32_t> " + el.id + ", " + (el.type == "bool" ? "trig_" : "cv_") + el.id + ";\n";
}

function cppSource1(el){
    let s = 'if(id.compare("' + el.id + '") == 0){\n';
    s += '\tif(key.compare("current") == 0){\n';
    s += '\t\t' + el.id + ' = val;\n';
    s += '\t\treturn;\n';
    s += '\t}';
    s += 'else if(key.compare("';
    if(el.type == 'bool'){
        s += 'trig") == 0){\n';
        s += '\t\tif(val >= -1 && val <= 1)\n';
        s += '\t\t\ttrig_' + el.id + ' = val;\n';
    }else{
        s += 'cv") == 0){\n';
        s += '\t\tif(val >= -1 && val <= 3)\n';
        s += '\t\t\tcv_' + el.id + ' = val;\n';
    }
    //s += '\t\treturn;\n';
    s += '\t}\n\treturn;\n}\n';
    sCpp1 += s;
}

function cppSource2(el){
    let s = el.id + ' = model->GetParamValue("' + el.id + '", "current");\n';
    s += (el.type == 'bool' ? 'trig_' : 'cv_') + el.id + ' = model->GetParamValue("' + el.id + '", "' + (el.type == 'bool' ? 'trig' : 'cv') + '");\n';
    sCpp2 += s;
}

function smpSource(el){
    let o = {};
    o.id = el.id;
    if(el.type == 'bool'){
        o.current = 0;
        o.trig = -1;
    }else if(el.type == 'int'){
        o.current = Math.floor((el.max - el.min)/2);
        o.cv = -1;
    }
    smp.patches[0].params.push(o);
}

