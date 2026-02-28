const Logic = require('./build/logic.js').default;

Logic().then(logic => {
  console.log(logic);
  logic.logicRun({ message: "Hello from JavaScript!" });
}).catch(err => {
    console.error(err);
});
