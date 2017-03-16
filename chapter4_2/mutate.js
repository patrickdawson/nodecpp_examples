"use strict";
const addon = require('./cpp/build/Release/mutate');

var obj = { x: 0 };
addon.start(obj);
console.log("started...")
setInterval(() => {
    console.log(obj)
}, 1000);

