function get() {
    return window.selected && window.selected.index;
}
get.toString = function () {
    return 'function(){return window.selected&&window.selected.index}'
}

function set() {
}
set.toString = function () {
    return 'function(){}';
}

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
