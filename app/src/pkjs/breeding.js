function get() {
    return this.v || {
        monsters: ['', '', '', '', '', '', ''],
        status_power: '0',
        status_tier: '1',
        status_steps: '10000'
    };
}
// get.toString = function () {
//     return 'function(){}';
// };

function set(value) {
    this.v = value;
    if (!this.initialized) {
        return;
    }

    var key, monster, owned = {}, breeding = {};

    var isOwned = function (monster) {
        var offset = 0, index = monster.index;
        while (index > 31) {
            offset++;
            index -= 32;
        }
        return (parseInt(value.monsters[offset]) || 0) & (1 << index);
    }
    var isBreed = function (monster, families) {
        if (monster.tier && value.status_tier >= monster.tier) {
            return true;
        }
        for (var i = 0; i < monster.breeding.length; i++) {
            var breeds = monster.breeding[i];
            var toBreed = true;
            for (var j = 0; j < breeds.length; j++) {
                var breed = breeds[j], checkFamily = breed.indexOf('-family');
                if (checkFamily > 0) {
                    if (!families[breed.substring(0, checkFamily)].owned.length) {
                        toBreed = false;
                        break;
                    }
                } else if (!owned[breed]) {
                    toBreed = false;
                    break;
                }
            }
            if (toBreed) {
                return true;
            }
        }
        return false;
    }

    for (key in this.monsters) {
        monster = this.monsters[key];
        if (monster.index && isOwned(monster)) {
            owned[key] = true;
            this.families[monster.family].owned.push(monster);
        }
    }
    for (key in this.monsters) {
        monster = this.monsters[key];
        if (!monster.index || owned[key]) {
            continue;
        }
        if (isBreed(monster, this.families)) {
            breeding[monster.index] = true;
        }
        this.families[monster.family].other.push(monster);
    }

    var createButtonElement = function (family) {
        var button = document.createElement('button');
        button.type = 'button';
        button.innerHTML = family.name;
        return button;
    }
    var createMonsterElement = function (monster) {
        var div = document.createElement('div');
        if (monster.power > value.status_power) {
            div.className = 'breeding--disabled';
        } else {
            div.addEventListener('click', function () {
                if (this.className) {
                    this.className = '';
                    window.selected = null;
                    return;
                }
                document.querySelectorAll('.breeding--selected').forEach(function (element) {
                    element.className = '';
                });
                this.className = 'breeding--selected';
                window.selected = monster;
            });
        }

        var img = document.createElement('img');
        img.src = 'data:image;base64,' + monster.image;
        div.appendChild(img);

        if (monster.power > 1) {
            var span = document.createElement('span');
            span.innerHTML = '+' + monster.power;
            span.className = 'breeding__power'
            div.appendChild(span);
        }

        return div;
    }

    var buttonWrapper = document.createElement('div');
    buttonWrapper.className = 'breeding_buttons';
    var breedingWrapper = document.createElement('div');
    for (key in this.families) {
        var family = this.families[key];

        var button = createButtonElement(family);
        button.dataset.id = 'breeding-' + key;
        var breedingElement = document.createElement('div');
        breedingElement.id = 'breeding-' + key;
        breedingElement.className = 'breeding--hidden';

        button.addEventListener('click', function () {
            document.querySelectorAll('.breeding--visible').forEach(function (element) {
                element.className = 'breeding--hidden';
            });
            document.querySelectorAll('.breeding--active').forEach(function (element) {
                element.className = '';
            });
            this.className = 'breeding--active';
            document.getElementById(this.dataset.id).className = 'breeding--visible';
        });

        var ownedElement = document.createElement('div')
        ownedElement.className = 'breeding__icons';
        var otherElement = document.createElement('other')
        otherElement.className = 'breeding__icons';

        family.owned.forEach(function (monster) {
            var div = createMonsterElement(monster);
            ownedElement.appendChild(div);
        });
        family.other.forEach(function (monster) {
            var div;
            if (breeding[monster.index]) {
                div = createMonsterElement(monster);
            } else {
                div = document.createElement('div');
                div.className = 'breeding--disabled';
                div.innerHTML = '<span class="breeding__unknown">?</span>';
            }
            otherElement.appendChild(div);
        });

        breedingElement.appendChild(ownedElement);
        if (otherElement.children.length) {
            var infoElement = document.createElement('p');
            infoElement.innerHTML = '<br/>New:'
            breedingElement.appendChild(infoElement);
        }
        breedingElement.appendChild(otherElement);

        buttonWrapper.appendChild(button);
        breedingWrapper.appendChild(breedingElement);
    }

    var base = document.createElement('div');

    var status = document.createElement('div');
    status.innerHTML += '<p>Tier: ' + value.status_tier + ' (' + value.status_steps + ' steps left)</p>';
    status.innerHTML += '<p>Power: ' + value.status_power + '</p>';
    status.innerHTML += '<br/>';
    base.appendChild(status);

    base.appendChild(buttonWrapper);
    base.appendChild(breedingWrapper);
    document.getElementById('breeding').appendChild(base);
}
// set.toString = function () {
//     return 'function(){}';
// };

function setData(userData) {
    this.initialized = true;
    this.monsters = userData.monsters;
    this.families = userData.families;
    if (this.v) {
        this.set(this.v);
    }
}
// setData.toString = function () {
//     return 'function(){}';
// };

var component = {
    name: 'breeding',
    template: '<div id="breeding" class="component breeding"></div>',
    manipulator: {get: get, set: set, setData: setData},
    style:
        '.breeding_buttons { display: flex; flex-wrap: wrap; }' +
        '.breeding_buttons button { margin: .25rem; border: 1px solid black; min-width: 100px; }' +
        '.breeding__icons { display: flex; flex-wrap: wrap; margin-top: 1rem; }' +
        '.breeding__icons > div { margin: .25rem; display: flex; align-items: center; justify-content: center; background: white; border-radius: .25rem; width: 73px; height: 73px; }' +
        '.breeding__icons > div { position: relative; border: 3px solid white; }' +
        '.breeding__power { position: absolute; top: 2px; right: 3px; font-size: 12px; line-height: 1; color: black; }' +
        '.breeding__unknown { font-size: 32px; color: rgba(0, 0, 0, .2); }' +
        '.breeding--active.breeding--active { border-color: white; }' +
        '.breeding--hidden { display: none; }' +
        '.breeding--disabled { opacity: .2; }' +
        '.breeding--selected.breeding--selected { border-color: lightgreen; }' +
        '#main-form > .component:last-child, #main-form > .section:last-child { margin-bottom: 80px; }'
};

module.exports = {
    init: function (clay) {
        clay.registerComponent(component);
        clay.setSettings('breeding', {
            monsters: [
                localStorage.getItem('breeding0'),
                localStorage.getItem('breeding1'),
                localStorage.getItem('breeding2'),
                localStorage.getItem('breeding3'),
                localStorage.getItem('breeding4'),
                localStorage.getItem('breeding5'),
                localStorage.getItem('breeding6')
            ],
            status_power: localStorage.getItem('status_power') || '0',
            status_tier: localStorage.getItem('status_tier') || '1',
            status_steps: localStorage.getItem('status_steps') || '10000'
        });
    },
    pebble: function (clay, payload) {
        var breeding =  {
            monsters: [
                payload['breeding0'].toString(),
                payload['breeding1'].toString(),
                payload['breeding2'].toString(),
                payload['breeding3'].toString(),
                payload['breeding4'].toString(),
                payload['breeding5'].toString(),
                payload['breeding6'].toString()
            ],
            status_power: payload['status_power'].toString(),
            status_tier: payload['status_tier'].toString(),
            status_steps: payload['status_steps'].toString()
        };
        localStorage.setItem('breeding0', breeding.monsters[0]);
        localStorage.setItem('breeding1', breeding.monsters[1]);
        localStorage.setItem('breeding2', breeding.monsters[2]);
        localStorage.setItem('breeding3', breeding.monsters[3]);
        localStorage.setItem('breeding4', breeding.monsters[4]);
        localStorage.setItem('breeding5', breeding.monsters[5]);
        localStorage.setItem('breeding6', breeding.monsters[6]);
        localStorage.setItem('status_power', breeding.status_power);
        localStorage.setItem('status_tier', breeding.status_tier);
        localStorage.setItem('status_steps', breeding.status_steps);
        clay.setSettings('breeding', breeding);
    },
    userData: {
        monsters: require('./monsters.json'),
        families: require('./families.json')
    }
};
