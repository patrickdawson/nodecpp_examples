const mutate = require('./build/Release/mutate');

var myObj = {};
mutate.mutate(myObj);

console.log(JSON.stringify(myObj));