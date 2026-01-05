# GraniteBox Justification

GraniteBox was started by Alexander Williams-Letondre in an attempt to openly rebel against the modern desktop application development idiom of throwing frameworks on frameworks until the stack overflows. (Hah)

That means it doesn't use a browser, a javascript runtime, or defered rendering (a'la parsing an HTML file) unless a developer explicitly bakes it in as an extension to their specific application implementation details.


### Micro-Kernel

A micro-kernel doesn't really have a specific, terse, definition. But, roughly it can be thought of as a kernel-like set of raw utilities that define basic application runtime loops.

A classical micro-kernel implementation is QT's literally-defined "kernel" utilities. Which, act as the bedrock of all higher-level abstractions in their codebase.

Another example implementation is in the embedded systems world, through the amazon "freeRTOS" project, which implements micro kernels that are developer-orchestrated.

The latter is more closely related to this project then the prior, since the framework is designed to allow developers to closely integrate with internals for performance, but it's important to note related implementations for completeness.


### Linux as a Framework

This is probably the most unique statement out of everything previously discussed. And, arguably, likely the primary reason you are reading the docs to begin with. In a traditional desktop application, developers implement or make use of cross-platform libraries which allow binaries to be executed on different OS platforms.


#### Windows as a Runtime

Historically, the dominant operating system was microsoft windows. So, most early applications would use the .NET stack and C# alongside the windows development kit (WDK) to build applications. However, there are a couple of problems with this:

1. Everything is proprietary and reliant on microsoft support.
2. Tooling is terrible.
3. Inconsitencies and bad (typically legacy) decisions are everywhere.
4. C# (Discussed below)

C# is not exactly condusive to high-performance programming due to it's reliance on garbage collection. There is also a distinct lack of determinism. Meaning, bugs can sometimes creep in, security audits fail to succeed, and quality assurance toolkits / checkers are often non-strict.


#### Developer vs. Consumer Preference

Interestingly, despite the consumer preference for the windows operating system, quite a few modern developers focused on the system-level prefer the Linux kernel and related distributions. The reasons for this are as follows:

1. Everything is Open Source. // Find an issue? Debug it! How does it work? Look at the source code!
2. Tooling is amazing. // gcc or g++ vs VS C/C++ tooling powershell script hell.
3. Scalability / Customizability // Yocto project, custom distributions, debian / redhat environments, modules and subsystems... Need I go on?
4. C/C++/Rust

Obviously, developers would prefer to use Linux for these reasons. And obviously, consumers chose windows due to reasons completely unrelated.

The issues involved with this became so extensive, that eventually developers decided to rent their own computers from massive tech companies to run software on their own box, and use a platform designed for viewing deferred-rendering simple webpages as application development frameworks. That's a lot of overhead to make text appear when someone click's their screen!


### So... Linux as a Framework?

I'm getting to it.

The solution to the above problems eventually became writing everything in windows, and developing tools like Wine to handle syscall-level translation between the operating systems. In effect, old application would be built for windows, and run on Linux. Alternatives that became prevelant include Qt for general purpose applications, which is hard to justify due to licensing concerns, or custom GUI / Windowing toolkits for bigger teams using various cross-platforms solutions such as GTK or wxwidgets, which have their own problems.

But all of these systems we build up, abstractions we define, and platforms we setup, all fundamentally have one purpose.

We really like Linux. Developers *crave* the linux system and models. It's extremely appealing to hack things together yourself, understand explicitly why things happen and how, and to contribute to a system much larger than oneself whenever it is convenient. And never more.

So my query is, (and the whole point of this project is): What if we reversed Wine? What if we took a system like Docker or WSL2, and reduced it down to it's constituent goopy bits? We can write a framework that fully implements a linux translation layer using open source projects like WSL, reduce all the user-level stuff and runtime stuff that linux implements. (no need for a bash terminal if we are just writing to the linux API) and build applications that are fast, efficient, and lean?

And that's this project.

Amusingly, that's an...


### Application Development Kit

... Or a means of writing cross-platform applications using linux as a set of apis, and a basis that we can implement a runtime on, and translate syscalls to windows equivalents using WSL or adjacent.


