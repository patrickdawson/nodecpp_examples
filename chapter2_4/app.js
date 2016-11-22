"use strict";
const addon = require('./build/Release/addon');

let target = { x: 3 };

addon.init(target, 10);
console.log(target);

for (let i = 0; i < 3; ++i) {
    addon.increment();
    console.log(target);
}