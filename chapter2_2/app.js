const addon = require('./build/Release/addon');

console.log("Numbers");
console.log("-------");
console.log(addon.passNumber("2"));
console.log(addon.passNumber(2));

console.log("\nInteger");
console.log("-------");
console.log(addon.passInteger(2));
console.log(addon.passInteger(3.7));

console.log("\nBool");
console.log("-------");
console.log(addon.passBoolean(true));
console.log(addon.passBoolean("some"));
console.log(addon.passBoolean(2));
console.log(addon.passBoolean(false));
console.log(addon.passBoolean(null));
console.log(addon.passBoolean(undefined));

console.log("\nString");
console.log("-------");
console.log(addon.passString("hello"));
