const {test} = require('node:test');
const assert = require('node:assert/strict');
const vm = require('node:vm');
const fs = require('node:fs');
const source = fs.readFileSync('Lithophane_Light/Lithophane_Light.ino', 'utf8');
const script = source.match(/<script>([\s\S]*?)<\/script>/)[1];
const tick = () => new Promise(resolve => setImmediate(resolve));

function harness() {
  const elements = new Map();
  const get = id => {
    if (!elements.has(id)) elements.set(id, {
      value: ({mode:'0',brightness:'120',speed:'50',strength:'75',color:'#ffe0b0'})[id] || '',
      disabled: id === 'controls', style: {}, classList: {toggle(){}},
      appendChild(){}, setAttribute(){}
    });
    return elements.get(id);
  };
  const requests = [];
  const context = vm.createContext({
    document: {getElementById:get, createElement:()=>({style:{},setAttribute(){}}), querySelectorAll:()=>[]},
    URLSearchParams, setTimeout, clearTimeout,
    fetch: (url, options) => new Promise((resolve,reject) => requests.push({url,options,resolve,reject}))
  });
  vm.runInContext(script, context);
  return {get, requests, run: code => vm.runInContext(code,context)};
}

async function initialize(h) {
  h.requests[0].resolve({ok:true,json:async()=>({mode:2,brightness:180,speed:60,strength:70,color:'ABCDEF',power:0})});
  await tick();
}

test('controls wait for loaded state and reflect saved values', async () => {
  const h = harness();
  assert.equal(h.get('controls').disabled,true);
  assert.equal(h.requests[0].options.cache,'no-store');
  await initialize(h);
  assert.equal(h.get('controls').disabled,false);
  assert.equal(h.get('color').value,'#ABCDEF');
  assert.equal(h.get('brightness').value,180);
  assert.equal(h.get('power').textContent,'Valgustus väljas');
});

test('failed startup keeps controls disabled with reload guidance', async () => {
  const h = harness();
  h.requests[0].reject(new Error('offline'));
  await tick();
  assert.equal(h.get('controls').disabled,true);
  assert.match(h.get('status').textContent,/Laadi leht uuesti/);
});

test('writes use POST and custom header; overlapping changes coalesce to latest state', async () => {
  const h = harness(); await initialize(h);
  h.run('send()');
  h.get('brightness').value='42'; h.run('send(); send()');
  assert.equal(h.requests.length,2);
  const first = h.requests[1];
  assert.equal(first.url,'/set');
  assert.equal(first.options.method,'POST');
  assert.equal(first.options.headers['X-Lithophane-Control'],'1');
  first.resolve({ok:true}); await tick();
  assert.equal(h.requests.length,3);
  assert.equal(h.requests[2].options.body.get('brightness'),'42');
  h.requests[2].resolve({ok:true}); await tick();
  assert.equal(h.get('status').textContent,'Seadistatud');
});

test('failed update reports error and permits a subsequent update', async () => {
  const h = harness(); await initialize(h);
  h.run('send()'); h.requests[1].resolve({ok:false,status:400}); await tick();
  assert.equal(h.get('status').textContent,'Ühenduse viga');
  h.run('send()');
  assert.equal(h.requests.length,3);
  h.requests[2].resolve({ok:true}); await tick();
  assert.equal(h.get('status').textContent,'Seadistatud');
});
