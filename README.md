<div align="center">
  <img src="src/qt/res/icons/tessera.png" alt="Tessera" width="120" />

  <h1>Tessera</h1>

  <p><strong>A post-quantum cryptocurrency built on the Bitcoin Core architecture.</strong></p>

  [![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](COPYING)
</div>

---

Tessera keeps the Bitcoin Core design — a UTXO ledger, Script, full-node
validation, a peer-to-peer network, a built-in wallet — and replaces the
cryptography that a quantum computer breaks.

That replacement is the whole point. In March 2026 a joint whitepaper from
Google Quantum AI, the Ethereum Foundation and Stanford put a number on it: the
256-bit elliptic-curve discrete logarithm, the problem every Bitcoin signature
rests on, falls in minutes to fewer than half a million physical qubits
([arXiv:2603.28846](https://arxiv.org/abs/2603.28846)). NIST has separately
scheduled ECDSA for deprecation after 2030 and disallowance after 2035. Tessera
starts on the other side of that line.

### What is different from Bitcoin

- **Signatures: ML-DSA-44** (FIPS 204), vendored from PQClean. There is no
  elliptic-curve code in the tree at all — `libsecp256k1` is not present.
- **Encrypted transport: ML-KEM-768** (FIPS 203) in the v2 P2P handshake, in
  place of the elliptic-curve exchange of BIP324. On by default, so the wire is
  covered as well as the coins.
- **Hashing: SHA3-256** (FIPS 202) throughout — block and transaction ids,
  Merkle trees — and **SHA3-256d** for proof of work.
- **Everything else is deliberately unchanged.** The 2016-block difficulty
  retarget, the ten-minute target, the 210 000-block halving, Script. A
  post-quantum chain is already enough novelty for one project.

The cost is honest and worth stating: an ML-DSA-44 input is 3732 bytes against
104 for ECDSA, so a transaction is roughly 17× a Bitcoin transaction, and a
payment costs proportionally more in fees at equal congestion. Nothing about the
signature scheme is free.

## Status

**Not launched.** The genesis blocks are mined and the parameters are fixed, but
no public network is running yet. Consensus rules may still change before launch;
after it, they will not.

The chain begins at **difficulty 1** — the same target Bitcoin's genesis used —
so an ordinary CPU can mine from the first block. On a modern desktop that is a
few minutes per block until the difficulty finds its level.

## Binaries

| Binary | Purpose |
| --- | --- |
| `tesserad` | Headless full node and wallet daemon |
| `tessera-cli` | JSON-RPC command-line client |
| `tessera-qt` | Desktop GUI wallet (Qt Widgets) |
| `tessera-miner` | Standalone multi-threaded CPU miner |
| `tessera-wallet` | Offline wallet tool |
| `tessera-tx` | Offline raw-transaction tool |
| `tessera-util` | Offline utility tool |

## Building

CMake and a C++26 compiler (GCC ≥ 14 or Clang ≥ 19). Per-platform instructions
and the dependency list are in [`doc/build-unix.md`](doc/build-unix.md),
[`doc/build-windows.md`](doc/build-windows.md) and
[`doc/dependencies.md`](doc/dependencies.md).

```bash
# deps (Ubuntu 24.04): g++-14 cmake libboost-dev libsqlite3-dev libzmq3-dev
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"
ctest --test-dir build              # unit tests
```

The wallet and ZMQ notifications are on by default. The Qt GUI
(`-DENABLE_GUI=ON`) and multiprocess IPC (`-DENABLE_IPC=ON`, Unix only) are
opt-in.

## Running

```bash
# regtest — a private network for development
tesserad -regtest -daemon
tessera-cli -regtest getblockchaininfo
tessera-cli -regtest -generate 10
tessera-cli -regtest getnewaddress
```

## Mining

`tessera-miner` fetches work over JSON-RPC, grinds SHA3-256d on every core and
submits the block. It is a plain CPU miner: no GPU code ships in this tree, and
anyone who wants one is free to write it.

```bash
tessera-miner -benchmark                       # this machine's hashrate
tessera-miner -address=<your address>          # mine to a running tesserad
tessera-miner -address=<addr> -threads=4       # or pin the thread count
```

The node's own `generatetoaddress` RPC exists too, but it is single-threaded and
gives up after a million attempts by default — about 0.02% of what one block
needs at difficulty 1. It is meant for regtest, not for mining.

## Network parameters

| | mainnet | testnet | regtest |
| --- | --- | --- | --- |
| P2P port | 11333 | 21333 | 21444 |
| RPC port | 11332 | 21332 | 21443 |
| Address prefix | `T` | `t` | `t` |
| bech32 prefix | `tsr` | `ttsr` | `tsrrt` |
| Data directory | `~/.tessera` | | |

## Repository layout

```
src/            node, consensus, wallet, P2P, RPC, crypto
  ML-DSA-44/    vendored FIPS 204 signature implementation
  ML-KEM-768/   vendored FIPS 203 key encapsulation
  qt/           the Qt Widgets GUI
  test/         unit tests (run via ctest)
tools/          offline helpers, including the genesis miner
doc/            build, design and interface documentation
contrib/        tooling and helper scripts
```

## Documentation

Start at [`doc/README.md`](doc/README.md). Design notes are under
[`doc/design/`](doc/design/).

## Contributing and security

Developer notes and coding style: [`doc/developer-notes.md`](doc/developer-notes.md).

Report vulnerabilities as described in [`SECURITY.md`](SECURITY.md) — never in a
public issue.

## License

MIT. See [`COPYING`](COPYING).
