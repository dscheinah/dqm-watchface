function get() {
    return window.selected && window.selected.index;
}
// get.toString = function () {
//     return 'function(){}'
// }

function set() {
}
// set.toString = function () {
//     return 'function(){}';
// }

var component = {
    name: 'selection',
    template: '',
    manipulator: {get: get, set: set}
};

module.exports = {
    init: function (clay) {
        clay.registerComponent(component);
    }
};
