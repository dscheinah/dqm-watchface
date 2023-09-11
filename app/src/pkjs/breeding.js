function get() {
    return this.v || {
        monsters: ['', '', '', '', '', '', ''],
        status_power: '0',
        status_tier: '1',
        status_steps: '10000'
    };
}
get.toString = function () {
    return 'function(){return this.v||{monsters:["","","","","","",""],status_power:"0",status_tier:"1",status_steps:"10000"}}';
};

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
    buttonWrapper.className = 'breeding__buttons';
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
set.toString = function () {
    return 'function(e){if(this.v=e,this.initialized){var n,i,t={},r={},a=function(n){for(var i=0,t=n.index;t>31;)i++,t-=32;return(parseInt(e.monsters[i])||0)&1<<t},d=function(n,i){if(n.tier&&e.status_tier>=n.tier)return!0;for(var r=0;r<n.breeding.length;r++){for(var a=n.breeding[r],d=!0,s=0;s<a.length;s++){var c=a[s],l=c.indexOf("-family");if(l>0){if(!i[c.substring(0,l)].owned.length){d=!1;break}}else if(!t[c]){d=!1;break}}if(d)return!0}return!1};for(n in this.monsters)(i=this.monsters[n]).index&&a(i)&&(t[n]=!0,this.families[i.family].owned.push(i));for(n in this.monsters)(i=this.monsters[n]).index&&!t[n]&&(d(i,this.families)&&(r[i.index]=!0),this.families[i.family].other.push(i));var s=function(e){var n=document.createElement("button");return n.type="button",n.innerHTML=e.name,n},c=function(n){var i=document.createElement("div");n.power>e.status_power?i.className="breeding--disabled":i.addEventListener("click",(function(){if(this.className)return this.className="",void(window.selected=null);document.querySelectorAll(".breeding--selected").forEach((function(e){e.className=""})),this.className="breeding--selected",window.selected=n}));var t=document.createElement("img");if(t.src="data:image;base64,"+n.image,i.appendChild(t),n.power>1){var r=document.createElement("span");r.innerHTML="+"+n.power,r.className="breeding__power",i.appendChild(r)}return i},l=document.createElement("div");l.className="breeding__buttons";var o=document.createElement("div");for(n in this.families){var m=this.families[n],u=s(m);u.dataset.id="breeding-"+n;var h=document.createElement("div");h.id="breeding-"+n,h.className="breeding--hidden",u.addEventListener("click",(function(){document.querySelectorAll(".breeding--visible").forEach((function(e){e.className="breeding--hidden"})),document.querySelectorAll(".breeding--active").forEach((function(e){e.className=""})),this.className="breeding--active",document.getElementById(this.dataset.id).className="breeding--visible"}));var p=document.createElement("div");p.className="breeding__icons";var f=document.createElement("other");if(f.className="breeding__icons",m.owned.forEach((function(e){var n=c(e);p.appendChild(n)})),m.other.forEach((function(e){var n;r[e.index]?n=c(e):((n=document.createElement("div")).className="breeding--disabled",n.innerHTML=\'<span class="breeding__unknown">?</span>\'),f.appendChild(n)})),h.appendChild(p),f.children.length){var v=document.createElement("p");v.innerHTML="<br/>New:",h.appendChild(v)}h.appendChild(f),l.appendChild(u),o.appendChild(h)}var b=document.createElement("div"),g=document.createElement("div");g.innerHTML+="<p>Tier: "+e.status_tier+" ("+e.status_steps+" steps left)</p>",g.innerHTML+="<p>Power: "+e.status_power+"</p>",g.innerHTML+="<br/>",b.appendChild(g),b.appendChild(l),b.appendChild(o),document.getElementById("breeding").appendChild(b)}}';
};

function setData(userData) {
    this.initialized = true;
    this.monsters = userData.monsters;
    this.families = userData.families;
    if (this.v) {
        this.set(this.v);
    }
}
setData.toString = function () {
    return 'function(i){this.initialized=!0,this.monsters=i.monsters,this.families=i.families,this.v&&this.set(this.v)}';
};

var component = {
    name: 'breeding',
    template: '<div id="breeding" class="component breeding"></div>',
    manipulator: {get: get, set: set, setData: setData},
    style:
        '.breeding__buttons { display: flex; flex-wrap: wrap; }' +
        '.breeding__buttons button { margin: .25rem; border: 1px solid black; min-width: 100px; }' +
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
