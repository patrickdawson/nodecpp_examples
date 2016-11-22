const addon = require('./build/Release/mutate');

var myObj = {x:5};

console.log(JSON.stringify(myObj));
addon.mutate(myObj);
console.log(JSON.stringify(myObj));
