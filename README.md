# node-cryptonight-old-hardware
> node bindings for cryptonight hashing on old hardware that does not support AES-NI extensions

Cryptonight hashing functions for node.js.

Recommended for usage on old hardware, prefer using [node-cryptonight](https://github.com/ExcitableAardvark/node-cryptonight) on modern hardware and production.

Supports cryptonight variant: 0, 1 and 2

### Installation

`npm install --save node-cryptonight-old-hardware`
   
### Testing

Code is linted with [standard](https://github.com/standard/standard) and tested with [Jest](https://github.com/facebook/jest). Run `npm test` to lint and run tests.

### Usage Examples

##### Synchronous Hashing

```js
const cryptonight = require('node-cryptonight').hash
const hash = cryptonight(Buffer.from('This is a test'))
console.log(hash) // <Buffer a0 84 f0 1d 14 37 ..>
```

##### Synchronous Hashing with variant 1

```js
const cryptonight = require('node-cryptonight').hash
const hash = cryptonight(Buffer.from('This is a test'), 1)
console.log(hash) // <Buffer a0 84 f0 1d 14 37 ..>
```

##### Asynchronous Hashing

```js
const cryptonight = require('node-cryptonight').asyncHash
cryptonight(Buffer.from('This is a test'), hash => {
  console.log(hash) // <Buffer a0 84 f0 1d 14 37 ..>
})
```
##### Asynchronous Hashing with variant 1

```js
const cryptonight = require('node-cryptonight').asyncHash
cryptonight(Buffer.from('This is a test'), 1, hash => {
  console.log(hash) // <Buffer a0 84 f0 1d 14 37 ..>
})
```

##### Promise Wrapper Example

```js
function cryptonight(data) {
  return new Promise((resolve, reject) => {
    require('node-cryptonight').asyncHash(data, hash => {
      resolve(hash)
    })
  })
}

cryptonight(Buffer.from('This is a test'))
  .then(console.log) // <Buffer a0 84 f0 1d 14 37 ..>
```


### See Also

Following libraries have similar API with AES-NI support:

* [node-cryptonight](https://github.com/ExcitableAardvark/node-cryptonight)
* [node-cryptonight-lite](https://github.com/ExcitableAardvark/node-cryptonight-lite)

Hashing algorithm implementations are provided by:

* [xmrig](https://github.com/xmrig)

### License

Released under the 3-Clause BSD License. Contains code from the Monero project. See `LICENSE` for more information.
