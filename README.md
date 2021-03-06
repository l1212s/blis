![The BLIS cat is sleeping.](http://www.cs.utexas.edu/users/field/blis_cat.png)

[![Build Status](https://travis-ci.org/flame/blis.svg?branch=master)](https://travis-ci.org/flame/blis)


Introduction
------------

BLIS is a portable software framework for instantiating high-performance
BLAS-like dense linear algebra libraries. The framework was designed to isolate
essential kernels of computation that, when optimized, immediately enable
optimized implementations of most of its commonly used and computationally
intensive operations. BLIS is written in [ISO
C99](http://en.wikipedia.org/wiki/C99) and available under a
[new/modified/3-clause BSD
license](http://opensource.org/licenses/BSD-3-Clause). While BLIS exports a
[new BLAS-like API](docs/BLISTypedAPI.md),
it also includes a BLAS compatibility layer which gives application developers
access to BLIS implementations via traditional [BLAS routine
calls](http://www.netlib.org/lapack/lug/node145.html).
An [object-based API](docs/BLISObjectAPI.md) unique to BLIS is also available.

For a thorough presentation of our framework, please read our
journal article, ["BLIS: A Framework for Rapidly Instantiating BLAS
Functionality"](http://www.cs.utexas.edu/users/flame/pubs/blis1_toms_rev3.pdf).
For those who just want an executive summary, please see the next section.

In a follow-up article, ["The BLIS Framework: Experiments in
Portability"](http://www.cs.utexas.edu/users/flame/pubs/blis2_toms_rev3.pdf),
we investigate using BLIS to instantiate level-3 BLAS implementations on a
variety of general-purpose, low-power, and multicore architectures.

An IPDPS'14 conference paper titled ["Anatomy of High-Performance Many-Threaded
Matrix
Multiplication"](http://www.cs.utexas.edu/users/flame/pubs/blis3_ipdps14.pdf)
systematically explores the opportunities for parallelism within the five loops
that BLIS exposes in its matrix multiplication algorithm.

It is our belief that BLIS offers substantial benefits in productivity when
compared to conventional approaches to developing BLAS libraries, as well as a
much-needed refinement of the BLAS interface, and thus constitutes a major
advance in dense linear algebra computation. While BLIS remains a
work-in-progress, we are excited to continue its development and further
cultivate its use within the community. 

Key Features
------------

BLIS offers several advantages over traditional BLAS libraries:

 * **Portability that doesn't impede high performance.** Portability was a top
priority of ours when creating BLIS. With virtually no additional effort on the
part of the developer, BLIS is configurable as a fully-functional reference
implementation. But more importantly, the framework identifies and isolates a
key set of computational kernels which, when optimized, immediately and
automatically optimize performance across virtually all level-2 and level-3
BLIS operations. In this way, the framework acts as a productivity multiplier.
And since the optimized (non-portable) code is compartmentalized within these
few kernels, instantiating a high-performance BLIS library on a new
architecture is a relatively straightforward endeavor.

 * **Generalized matrix storage.** The BLIS framework exports interfaces that
allow one to specify both the row stride and column stride of a matrix. This
allows one to compute with matrices stored in column-major order, row-major
order, or by general stride. (This latter storage format is important for those
seeking to implement tensor contractions on multidimensional arrays.)
Furthermore, since BLIS tracks stride information for each matrix, operands of
different storage formats can be used within the same operation invocation. By
contrast, BLAS requires column-major storage. And while the CBLAS interface
supports row-major storage, it does not allow mixing storage formats. 

 * **Full support for the complex domain.** BLIS operations are developed and
expressed in their most general form, which is typically in the complex domain.
These formulations then simplify elegantly down to the real domain, with
conjugations becoming no-ops. Unlike the BLAS, all input operands in BLIS that
allow transposition and conjugate-transposition also support conjugation
(without transposition), which obviates the need for thread-unsafe workarounds.
Also, where applicable, both complex symmetric and complex Hermitian forms are
supported. (BLAS omits some complex symmetric operations, such as `symv`,
`syr`, and `syr2`.) 

 * **Advanced multithreading support.** BLIS allows multiple levels of
symmetric multithreading for nearly all level-3 operations. (Currently, users
may choose to obtain parallelism via either OpenMP or POSIX threads). This
means that matrices may be partitioned in multiple dimensions simultaneously to
attain scalable, high-performance parallelism on multicore and many-core
architectures. The key to this innovation is a thread-specific control tree
infrastructure which encodes information about the logical thread topology and
allows threads to query and communicate data amongst one another. BLIS also
employs so-called "quadratic partitioning" when computing dimension sub-ranges
for each thread, so that arbitrary diagonal offsets of structured matrices with
unreferenced regions are taken into account to achieve proper load balance.
More recently, BLIS introduced a runtime abstraction to specify parallelism on
a per-call basis, which is useful for applications that want to handle most of
the parallelism.

 * **Ease of use.** The BLIS framework, and the library of routines it
generates, are easy to use for end users, experts, and vendors alike. An
optional BLAS compatibility layer provides application developers with
backwards compatibility to existing BLAS-dependent codes. Or, one may adjust or
write their application to take advantage of new BLIS functionality (such as
generalized storage formats or additional complex operations) by calling one
of BLIS's native APIs directly. BLIS's typed API will feel familiar to many
veterans of BLAS since these interfaces use BLAS-like calling sequences. And
many will find BLIS's object-based APIs a delight to use when customizing
or writing their own BLIS operations. (Objects are relatively lightweight
`structs` and passed by address, which helps tame function calling overhead.) 

 * **Multilayered API, exposed kernels, and sandboxes.** The BLIS framework
exposes its
implementations in various layers, allowing expert developers to access exactly
the functionality desired. This layered interface includes that of the
lowest-level kernels, for those who wish to bypass the bulk of the framework.
Optimizations can occur at various levels, in part thanks to exposed packing
and unpacking facilities, which by default are highly parameterized and
flexible. And more recently, BLIS introduced sandboxes--a way to provide
alternative implementations of `gemm` that do not use any more of the BLIS
infrastructure than is desired. Sandboxes provide a convenient and
straightforward way of modifying the `gemm` implementation without disrupting
any other level-3 operation or any other part of the framework. This works
especially well when the developer wants to experiment with new optimizations
or try a different algorithm.

 * **Functionality that grows with the community's needs.** As its name
suggests, the BLIS framework is not a single library or static API, but rather
a nearly-complete template for instantiating high-performance BLAS-like
libraries. Furthermore, the framework is extensible, allowing developers to
leverage existing components to support new operations as they are identified.
If such operations require new kernels for optimal efficiency, the framework
and its APIs will be adjusted and extended accordingly. 

 * **Code re-use.** Auto-generation approaches to achieving the aforementioned
goals tend to quickly lead to code bloat due to the multiple dimensions of
variation supported: operation (i.e. `gemm`, `herk`, `trmm`, etc.); parameter
case (i.e. side, [conjugate-]transposition, upper/lower storage, unit/non-unit
diagonal); datatype (i.e. single-/double-precision real/complex); matrix
storage (i.e. row-major, column-major, generalized); and algorithm (i.e.
partitioning path and kernel shape). These "brute force" approaches often
consider and optimize each operation or case combination in isolation, which is
less than ideal when the goal is to provide entire libraries. BLIS was designed
to be a complete framework for implementing basic linear algebra operations,
but supporting this vast amount of functionality in a manageable way required a
holistic design that employed careful abstractions, layering, and recycling of
generic (highly parameterized) codes, subject to the constraint that high
performance remain attainable. 

 * **A foundation for mixed domain and/or mixed precision operations.** BLIS
was designed with the hope of one day allowing computation on real and complex
operands within the same operation. Similarly, we wanted to allow mixing
operands' floating-point precisions, or both domain and precision.
While this feature is not yet implemented, we plan to prototype and explore
the potential for adding mixed domain, mixed precision support to operations
such as `gemm`. 

Getting Started
---------------

If you just want to build/install a sequential (not parallelized) version of
BLIS in a hurry and come back and explore other topics later, you can configure
and build BLIS as follows:
```
$ ./configure auto
$ make [-j]
$ make install
```
You can then verify your build by running BLAS- and BLIS-specific test
drivers via `make check`:
```
$ make check [-j]
```
Please read the output of `./configure --help` for a full list of configure-time
options.
A more detailed walkthrough of the build system can be found in our
[Build System](docs/BuildSystem.md) guide.

We provide comprehensive documentation on BLIS's two primarily APIs:
the [object API](docs/BLISObjectAPI.md) and
the [typed API](docs/BLISTypedAPI.md).
These documents provide brief descriptions of each operation interface as
well as some more general information needed when developing an application
with BLIS.

If you want to begin using the object API in BLIS, please step through the
example code tutorial in the [examples/oapi](examples/oapi) directory.
We also have the equivalent code examples for the typed API available in
[examples/tapi](examples/tapi).

Users interested in using BLIS to obtain multithreaded parallelism should
read the [Multithreading](docs/Multithreading.md) documentation.

Have a quick question? You may find the answer in our list of [frequently asked
questions](docs/FAQ.md). 

Does BLIS contain kernels optimized for your favorite architecture? Please see
our [Hardware Support](docs/HardwareSupport.md) guide
for a full list of optimized kernels. 

The [Release Notes](docs/ReleaseNotes.md) contain a summary of new features
provided by each new tagged version (release) of BLIS, along with the date
the release.

We also provide documentation on the following topics, which will likely be of
interest to more advanced users and developers:
 * [Configurations](docs/ConfigurationHowTo.md).
This document describes how the configuration system works in BLIS, and also
provides step-by-step instructions for creating a new configuration.
(In BLIS, a "configuration" captures all of the details necessary to build
BLIS for a specific hardware architecture.) Configurations specify things
like cache blocksizes and kernel functions, as well as various optional
configuration settings. 
 * [Kernels](docs/KernelsHowTo.md).
This document describes each of the BLIS kernel operations in detail and should
provide developers with most of the information needed to get started with
writing and optimizing their own kernels. 
 * [Test suite](docs/Testsuite.md).
This document contains detailed instructions on running the BLIS test suite,
located in the top-level directory testsuite. Also included: a walkthrough
of the BLAS test drivers, which exercise the BLAS compatibility layer that
is, by default, included in BLIS.

A full listing of all documentation may be found via in the project's
[wiki](https://github.com/flame/blis/wiki) section.

External Linux packages
-----------------------

Generally speaking, we **highly recommend** building from source whenever possible using the latest `git` clone. (Tarballs of each [tagged release](https://github.com/flame/blis/releases) are also available, but are not preferred since they are more difficult to upgrade from than a git clone.)

If you prefer (or need) binary packages, please check out the following offerings available thanks to generous involvement/contributions from two of our community members.

 * Red Hat/Fedora. Dave Love provides rpm packages for x86_64, which he maintains at [Fedora Copr](https://copr.fedorainfracloud.org/coprs/loveshack/blis/).
 * Ubuntu/Debian. Nico Schlömer provides apt packages for various architectures, which he maintains at the PPA [launchpad.net](https://launchpad.net/%7Enschloe/+archive/ubuntu/blis-devel).

Discussion
----------

You can keep in touch with developers and other users of the project by joining
one of the following mailing lists:

 * [blis-devel](http://groups.google.com/group/blis-devel): Please join and
post to this mailing list if you are a BLIS developer, or if you are trying
to use BLIS beyond simply linking to it as a BLAS library.
**Note:** Most of the interesting discussions happen here; don't be afraid to
join! If you would like to submit a bug report, or discuss a possible bug,
please consider opening a [new issue](http://github.com/flame/blis/issues) on
github.

 * [blis-discuss](http://groups.google.com/group/blis-discuss): Please join and
post to this mailing list if you have general questions or feedback regarding
BLIS. Application developers (end users) may wish to post here, unless they
have bug reports, in which case they should open a
[new issue](http://github.com/flame/blis/issues) on github.

Contributing
------------

For information on how to contribute to our project, including preferred [coding conventions](docs/CodingConventions), please refer to the [CONTRIBUTING](CONTRIBUTING.md) file at the top-level of the BLIS source distribution.

Citations
---------

For those of you looking for the appropriate article to cite regarding BLIS, we
recommend citing our
[first ACM TOMS journal paper](http://dl.acm.org/authorize?N91172) 
([unofficial backup link](http://www.cs.utexas.edu/users/flame/pubs/blis1_toms_rev3.pdf)):

```
@article{BLIS1,
   author      = {Field G. {V}an~{Z}ee and Robert A. {v}an~{d}e~{G}eijn},
   title       = {{BLIS}: A Framework for Rapidly Instantiating {BLAS} Functionality},
   journal     = {ACM Transactions on Mathematical Software},
   volume      = {41},
   number      = {3},
   pages       = {14:1--14:33},
   month       = jun,
   year        = {2015},
   issue_date  = {June 2015},
   url         = {http://doi.acm.org/10.1145/2764454},
}
``` 

You may also cite the
[second ACM TOMS journal paper](http://dl.acm.org/authorize?N16240) 
([unofficial backup link](http://www.cs.utexas.edu/users/flame/pubs/blis2_toms_rev3.pdf)):

```
@article{BLIS2,
   author      = {Field G. {V}an~{Z}ee and Tyler Smith and Francisco D. Igual and
                  Mikhail Smelyanskiy and Xianyi Zhang and Michael Kistler and Vernon Austel and
                  John Gunnels and Tze Meng Low and Bryan Marker and Lee Killough and
                  Robert A. {v}an~{d}e~{G}eijn},
   title       = {The {BLIS} Framework: Experiments in Portability},
   journal     = {ACM Transactions on Mathematical Software},
   volume      = {42},
   number      = {2},
   pages       = {12:1--12:19},
   month       = jun,
   year        = {2016},
   issue_date  = {June 2016},
   url         = {http://doi.acm.org/10.1145/2755561},
}
``` 

We also have a third paper, submitted to IPDPS 2014, on achieving
[multithreaded parallelism in BLIS](http://www.cs.utexas.edu/users/flame/pubs/blis3_ipdps14.pdf):

```
@inproceedings{BLIS3,
   author      = {Tyler M. Smith and Robert A. {v}an~{d}e~{G}eijn and Mikhail Smelyanskiy and
                  Jeff R. Hammond and Field G. {V}an~{Z}ee},
   title       = {Anatomy of High-Performance Many-Threaded Matrix Multiplication},
   booktitle   = {28th IEEE International Parallel \& Distributed Processing Symposium
                  (IPDPS 2014)},
   year        = 2014,
}
```

A fourth paper, submitted to ACM TOMS, also exists, which proposes an
[analytical model](http://dl.acm.org/citation.cfm?id=2925987) 
([unofficial backup link](http://www.cs.utexas.edu/users/flame/pubs/TOMS-BLIS-Analytical.pdf))
for determining blocksize parameters in BLIS: 

```
@article{BLIS4,
   author      = {Tze Meng Low and Francisco D. Igual and Tyler M. Smith and
                  Enrique S. Quintana-Ort\'{\i}},
   title       = {Analytical Modeling Is Enough for High-Performance {BLIS}},
   journal     = {ACM Transactions on Mathematical Software},
   volume      = {43},
   number      = {2},
   pages       = {12:1--12:18},
   month       = aug,
   year        = {2016},
   issue_date  = {August 2016},
   url         = {http://doi.acm.org/10.1145/2925987},
}
```

A fifth paper, submitted to ACM TOMS, begins the study of so-called
[induced methods for complex matrix multiplication](http://www.cs.utexas.edu/users/flame/pubs/blis5_toms_rev2.pdf):

```
@article{BLIS5,
   author      = {Field G. {V}an~{Z}ee and Tyler Smith},
   title       = {Implementing High-performance Complex Matrix Multiplication via the 3m and 4m Methods},
   journal     = {ACM Transactions on Mathematical Software},
   volume      = {44},
   number      = {1},
   pages       = {7:1--7:36},
   month       = jul,
   year        = {2017},
   issue_date  = {July 2017},
   url         = {http://doi.acm.org/10.1145/3086466},
}
``` 

A sixth paper, submitted to ACM TOMS, revisits the topic of the previous
article and derives a [superior induced method](http://www.cs.utexas.edu/users/flame/pubs/blis6_toms_rev0.pdf):

```
@article{BLIS6,
   author      = {Field G. {V}an~{Z}ee},
   title       = {Implementing High-Performance Complex Matrix Multiplication via the 1m Method},
   journal     = {ACM Transactions on Mathematical Software},
   note        = {submitted}
}
``` 


Funding
-------

This project and its associated research was partially sponsored by grants from
[Microsoft](http://www.microsoft.com/), [Intel](http://www.intel.com/), [Texas
Instruments](http://www.ti.com/), and [AMD](http://www.amd.com/), as well as
grants from the [National Science Foundation](http://www.nsf.gov/) (Awards
CCF-0917167, ACI-1148125/1340293, CCF-1320112, and ACI-1550493).

_Any opinions, findings and conclusions or recommendations expressed in this
material are those of the author(s) and do not necessarily reflect the views of
the National Science Foundation (NSF)._

