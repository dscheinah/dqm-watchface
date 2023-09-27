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
        if (!monster.index) {
            if (new Date().toISOString().indexOf('-04-01') > 0) {
              breeding[monster.index] = true;
              for (var familyKey in this.families) {
                this.families[familyKey].other.push(monster);
              }
            }
            continue;
        }
        if (owned[key]) {
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
        img.src = 'http://dqm.dscheinah.de/monsters/' + monster.index + '.png';
        div.appendChild(img);

        var span = document.createElement('span');
        span.innerHTML = '+' + monster.power;
        span.className = 'breeding__power'
        div.appendChild(span);

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
            infoElement.innerHTML = 'New:'
            breedingElement.appendChild(infoElement);
        }
        breedingElement.appendChild(otherElement);

        buttonWrapper.appendChild(button);
        breedingWrapper.appendChild(breedingElement);
    }

    var base = document.createElement('div');

    var status = document.createElement('div');
    status.className = 'breeding__status';
    status.innerHTML += '<p>Tier: ' + value.status_tier + ' (' + value.status_steps + ' steps left)</p>';
    status.innerHTML += '<p>Power: ' + value.status_power + '</p>';
    base.appendChild(status);

    base.appendChild(buttonWrapper);
    base.appendChild(breedingWrapper);
    document.getElementById('breeding').appendChild(base);
}
set.toString = function () {
    return 'function(e){if(this.v=e,this.initialized){var n,i,t={},r={},a=function(n){for(var i=0,t=n.index;t>31;)i++,t-=32;return(parseInt(e.monsters[i])||0)&1<<t},s=function(n,i){if(n.tier&&e.status_tier>=n.tier)return!0;for(var r=0;r<n.breeding.length;r++){for(var a=n.breeding[r],s=!0,d=0;d<a.length;d++){var l=a[d],c=l.indexOf("-family");if(c>0){if(!i[l.substring(0,c)].owned.length){s=!1;break}}else if(!t[l]){s=!1;break}}if(s)return!0}return!1};for(n in this.monsters)(i=this.monsters[n]).index&&a(i)&&(t[n]=!0,this.families[i.family].owned.push(i));for(n in this.monsters)if((i=this.monsters[n]).index)t[n]||(s(i,this.families)&&(r[i.index]=!0),this.families[i.family].other.push(i));else if((new Date).toISOString().indexOf("-04-01")>0)for(var d in r[i.index]=!0,this.families)this.families[d].other.push(i);var l=function(e){var n=document.createElement("button");return n.type="button",n.innerHTML=e.name,n.addEventListener("click",(function(){document.querySelectorAll(".breeding--visible").forEach((function(e){e.className="breeding--hidden"})),document.querySelectorAll(".breeding--active").forEach((function(e){e.className=""})),this.className="breeding--active",document.getElementById(this.dataset.id).className="breeding--visible"})),n},c=function(n){var i=document.createElement("div");n.power>e.status_power?i.className="breeding--disabled":i.addEventListener("click",(function(){if(this.className)return this.className="",void(window.selected=null);document.querySelectorAll(".breeding--selected").forEach((function(e){e.className=""})),this.className="breeding--selected",window.selected=n}));var t=document.createElement("img");t.src="http://dqm.dscheinah.de/monsters/"+n.index+".png",i.appendChild(t);var r=document.createElement("span");return r.innerHTML="+"+n.power,r.className="breeding__power",i.appendChild(r),i},o=document.createElement("div");o.className="breeding__buttons";var m=document.createElement("div");for(n in this.families){var u=this.families[n],h=l(u);h.dataset.id="breeding-"+n;var p=document.createElement("div");p.id="breeding-"+n,p.className="breeding--hidden";var f=document.createElement("div");f.className="breeding__icons";var v=document.createElement("other");if(v.className="breeding__icons",u.owned.forEach((function(e){var n=c(e);f.appendChild(n)})),u.other.forEach((function(e){var n;r[e.index]?n=c(e):((n=document.createElement("div")).className="breeding--disabled",n.innerHTML=\'<span class="breeding__unknown">?</span>\'),v.appendChild(n)})),p.appendChild(f),v.children.length){var b=document.createElement("p");b.innerHTML="New:",p.appendChild(b)}p.appendChild(v),o.appendChild(h),m.appendChild(p)}var g=document.createElement("div"),E=document.createElement("div");E.className="breeding__status",E.innerHTML+="<p>Tier: "+e.status_tier+" ("+e.status_steps+" steps left)</p>",E.innerHTML+="<p>Power: "+e.status_power+"</p>",g.appendChild(E),g.appendChild(o),g.appendChild(m),document.getElementById("breeding").appendChild(g)}}';
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
        '.breeding__status { display: flex; justify-content: space-between; margin-bottom: .7rem; }' +
        '.breeding__buttons { display: flex; flex-wrap: wrap; margin: -.25rem; }' +
        '.breeding__buttons button { margin: .25rem; border: 1px solid black; min-width: 70px; flex-grow: 1; }' +
        '.breeding__icons { display: flex; flex-wrap: wrap; margin: .7rem -.25rem; }' +
        '.breeding__icons > div { margin: .25rem; display: flex; align-items: center; justify-content: center; background: white; border-radius: .25rem; width: 72px; height: 72px; }' +
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
