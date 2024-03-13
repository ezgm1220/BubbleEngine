# Crash Course: configuration

## Introduction

随着时间的推移，"EnTT "在许多方面几乎可以完全自定义。这些变量只是自定义其工作方式的众多方法之一。
在绝大多数情况下，用户不会有兴趣更改默认参数。对于所有其他情况，可在运行时调整程序库行为的可能配置列表如下。

## Definitions



所有选项都将作为编译器的参数（或编译单元内的用户自定义宏（如有需要））。
每个参数都会产生内部库定义。不建议尝试修改这些定义，因为无法保证它们会像下面的选项一样长期保持稳定。

### ENTT_NOEXCEPTION

定义此变量，但不赋值，以关闭 `EnTT` 中的异常处理。
这大致等同于设置编译器标志 `-fno-exceptions`，但也仅限于本库。

### ENTT_USE_ATOMIC

一般来说，`EnTT`不提供支持多线程的原语。许多功能无需任何显式控制就能被多个线程分割，用户自己就能知道是否需要同步点。
然而，有些功能用户并不容易使用，因此通过本定义使其成为线程安全的。

### ENTT_ID_TYPE

`entt::id_type`由该定义直接控制，并在库中广泛使用。
默认情况下，它的类型是 `std::uint32_t`。不过，用户可以根据需要定义不同的默认类型。

### ENTT_SPARSE_PAGE

众所周知，`EnTT`的 ECS 模块基于*稀疏集*。也许鲜为人知的是，稀疏数组是分页的，以减少内存使用量。
页面的默认大小（即包含的元素数量）为 4096，但用户可以根据实际情况进行调整。在任何情况下，所选值必须是 2 的幂次。

### ENTT_PACKED_PAGE

与稀疏数组一样，打包数组也是分页的。不过，在这种情况下，目的不是减少内存使用量，而是在创建组件时保证指针的稳定性。
页面的默认大小（即包含的元素数量）为 1024，但用户可以酌情调整。在任何情况下，所选值必须是 2 的幂次。

### ENTT_ASSERT

出于性能考虑，`EnTT` 不使用异常或任何其他控制结构。事实上，如果使用不当，它提供的许多功能都会导致未定义的行为。
为了解决这个问题，该库依赖于大量的断言，以便在调试构建时检测错误。默认情况下，它在内部使用 `assert`。用户可以通过设置该变量来覆盖其行为。

ENTT_ASSERT_CONSTEXPR

通常，在 `constexpr` 函数中使用断言并不是什么大问题。然而，在极端定制的情况下，区别对待可能是有用的。
为此，`EnTT` 引入了一个名字很难听的变量来简化这方面的工作。默认情况下，该变量会将参数转发给 `ENTT_ASSERT`。

ENTT_DISABLE_ASSERT

启用断言可能会在一定程度上影响性能。无论 `ENTT_ASSERT` 和 `ENTT_ASSERT_CONSTEXPR` 是否重新定义，都可以通过此定义立即禁用所有断言。
请注意，`ENTT_DISABLE_ASSERT` 优先于其他变量的重新定义，因此无论如何都会禁用所有控制。

### ENTT_NO_ETO

为了减少内存消耗和提高性能，`EnTT`的 ECS 模块从不实例化或存储空类型。
使用该变量可以将这些类型与其他类型一样对待，从而为其创建专用存储空间。

### ENTT_STANDARD_CPP

EnTT "将非标准语言特性与其他完全兼容的语言特性混合在一起，以提供部分功能。
该定义可防止库使用非标准技术，即不完全符合标准 C++ 的功能。
虽然在撰写本文时还没有已知的可移植性问题，但无论如何，如果需要的话，这都会使程序库具有完全的可移植性。

# Crash Course: containers

## Introduction

标准 C++ 库提供了广泛的容器，要想做得更好确实很难（不过要想做得更差也很容易，网上的许多示例就证明了这一点）。
EnTT并不试图以任何方式取代标准所提供的内容。恰恰相反，因为标准容器被广泛使用。
不过，该库也试图通过提供一些最初为内部使用而开发的容器来填补特性和功能上的空白。

库的这一部分可能会随着时间的推移而变大。但是，就目前而言，它很小，主要是为了满足一些内部需求。
对于所有可用的容器，可以像往常一样保证完整的测试覆盖率和长期稳定性。

## Containers

### Dense map

The dense map made available in `EnTT` is a hash map that aims to return a packed array of elements, so as to reduce the number of jumps in memory during iterations.
The implementation is based on *sparse sets* and each bucket is identified by an implicit list within the packed array itself.

在 `EnTT` 中提供的密集映射是一种哈希映射，旨在返回一个元素打包数组，以减少迭代过程中内存的跳转次数。
其实现基于*稀疏集*，每个桶都由打包数组中的隐式列表标识。

The interface is very close to its counterpart in the standard library, that is, the `std::unordered_map` class.
However, both local and non-local iterators returned by a dense map belong to the input iterator category although they respectively model the concepts of a *forward iterator* type and a *random access iterator* type.

该接口非常接近标准库中的对应接口，即 `std::unordered_map` 类。
不过，密集映射返回的本地和非本地迭代器都属于输入迭代器类别，尽管它们分别模拟了*前向迭代器*类型和*随机访问迭代器*类型的概念。

This is because they return a pair of references rather than a reference to a pair. In other words, dense maps return a so called *proxy iterator* the value type of which is:

这是因为它们返回的是一对引用，而不是一对引用。换句话说，密集映射返回一个所谓的*代理迭代器*，其值类型是：

- `std::pair<const Key &, Type &>` for non-const iterator types.
- `std::pair<const Key &, const Type &>` for const iterator types.

这与任何标准库地图返回的结果都大相径庭，因此在寻找可直接替换的地图时应加以考虑。

## Dense set

在 `EnTT` 中提供的密集集是一个哈希集合，旨在返回一个元素打包数组，以减少迭代过程中内存的跳转次数。
其实现基于*稀疏集*，每个桶都由打包数组中的隐式列表来标识。

该接口在所有方面都类似于标准库中的对应接口，即 `std::unordered_set` 类。
不过，这种集合也支持反向迭代，因此提供了所有必要的函数（如 `rbegin` 和 `rend`）。

# Crash Course: cooperative scheduler

## Introduction

Processes are a useful tool to work around the strict definition of a system and introduce logic in a different way, usually without resorting to other component types.
`EnTT` offers minimal support to this paradigm by introducing a few classes used to define and execute cooperative processes.

进程是一种有用的工具，可以绕过系统的严格定义，以不同的方式引入逻辑，通常无需借助其他组件类型。
EnTT`通过引入一些用于定义和执行合作进程的类，为这种范式提供了最低限度的支持。

## The process

A typical task inherits from the `process` class template that stays true to the CRTP idiom. Moreover, derived classes specify what the intended type for elapsed times is.

典型的任务继承自 "进程 "类模板，该模板与 CRTP 习惯保持一致。此外，派生类指定了已用时间的预期类型。

A process should expose publicly the following member functions whether needed (note that it isn't required to define a function unless the derived class wants to *override* the default behavior):

进程应根据需要公开暴露以下成员函数（注意，除非派生类想覆盖默认行为，否则不需要定义函数）：

- `void update(Delta, void *);`

  This is invoked once per tick until a process is explicitly aborted or ends either with or without errors. Even though it's not mandatory to declare this member function, as a rule of thumb each process should at least define it to work *properly*. The `void *` parameter is an opaque pointer to user data (if any) forwarded directly to the process during an update.
  每次运行都会调用一次，直到进程被明确中止或有错误或无错误地结束。尽管并不是必须声明此成员函数，但作为经验法则，每个进程至少应定义此成员函数，以便正常地工作。void *` 参数是一个不透明指针，指向更新过程中直接转发给进程的用户数据（如果有的话）。

- `void init();`

  This is invoked when the process joins the running queue of a scheduler. It happens usually as soon as the process is attached to the scheduler if it's a top level one, otherwise when it replaces its parent if it's a *continuation*.

  当进程加入调度程序的运行队列时调用。如果是顶级进程，通常是在进程加入调度程序后立即调用；如果是*延续*进程，则是在进程取代父进程后调用。

- `void succeeded();`

  This is invoked in case of success, immediately after an update and during the same tick.

  如果更新成功，则会在更新后立即调用，或在同一个 tick 中调用。

- `void failed();`

  This is invoked in case of errors, immediately after an update and during the same tick.

  在出现错误时、更新后立即和在同一个 tick 中都会调用。

- `void aborted();`

  This is invoked only if a process is explicitly aborted. There is no guarantee that it executes in the same tick, it depends solely on whether the process is aborted immediately or not.

  只有在进程被明确中止时才会调用。无法保证在同一个 tick 中执行，这完全取决于进程是否立即中止。

Derived classes can also change the internal state of a process by invoking `succeed` and `fail`, as well as `pause` and `unpause` the process itself.
All these are protected member functions made available to manage the life cycle of a process from a derived class.

派生类还可以通过调用 "成功 "和 "失败 "以及 "暂停 "和 "取消暂停 "来改变进程的内部状态。
所有这些都是受保护的成员函数，用于管理派生类中进程的生命周期。

Here is a minimal example for the sake of curiosity:

```C++
struct my_process: entt::process<my_process, std::uint32_t> {
    using delta_type = std::uint32_t;

    my_process(delta_type delay)
        : remaining{delay}
    {}

    void update(delta_type delta, void *) {
        remaining -= std::min(remaining, delta);

        // ...

        if(!remaining) {
            succeed();
        }
    }

private:
    delta_type remaining;
};
```

### Adaptor

Lambdas and functors can't be used directly with a scheduler because they aren't properly defined processes with managed life cycles.
This class helps in filling the gap and turning lambdas and functors into full-featured processes usable by a scheduler.

Lambdas 和函数不能直接与调度程序一起使用，因为它们不是具有可管理生命周期的适当定义的进程。
该类有助于填补这一空白，并将 lambdas 和函数转化为调度程序可以使用的全功能进程。

The function call operator has a signature similar to the one of the `update` function of a process but for the fact that it receives two extra callbacks to invoke whenever a process terminates with success or with an error:
函数调用运算符具有类似于进程的“update”函数的签名，但事实上，每当进程成功终止或出现错误时，它都会收到两个额外的回调来调用：

```C++
void(Delta delta, void *data, auto succeed, auto fail);
```

Parameters have the following meaning:

- `delta` is the elapsed time.
  `delta` 是经过的时间。
- `data` is an opaque pointer to user data if any, `nullptr` otherwise.
  如果有，`data` 是指向用户数据的不透明指针，否则为`nullptr`。
- `succeed` is a function to call when a process terminates with success.
  `succeed` 是当进程成功终止时调用的函数。
- `fail` is a function to call when a process terminates with errors.
  `fail` 是当进程出错终止时调用的函数。

Both `succeed` and `fail` accept no parameters at all.
`succeed`和`fail`都不接受任何参数。

Note that usually users shouldn't worry about creating adaptors at all. A scheduler creates them internally each and every time a lambda or a functor is used as a process.
需要注意的是，通常用户根本不需要担心创建适配器的问题。每次将 lambda 或函数作为进程使用时，调度程序都会在内部创建适配器。

## The scheduler

A cooperative scheduler runs different processes and helps managing their life cycles.
合作调度程序运行不同的进程，并帮助管理它们的生命周期。

Each process is invoked once per tick. If it terminates, it's removed automatically from the scheduler and it's never invoked again. Otherwise, it's a good candidate to run one more time the next tick.
A process can also have a *child*. In this case, the parent process is replaced with its child when it terminates and only if it returns with success. In case of errors, both the parent process and its child are discarded. This way, it's easy to create chain of processes to run sequentially.
每个进程每个 tick 调用一次。如果它终止了，就会自动从调度程序中删除，并且再也不会被调用。否则，它就会在下一个 tick 时再运行一次。
一个进程也可以有一个*子进程*。在这种情况下，父进程在终止时，只有在成功返回时才会被子进程替换。如果出现错误，父进程和子进程都会被丢弃。这样，就很容易创建顺序运行的进程链。

Using a scheduler is straightforward. To create it, users must provide only the type for the elapsed times and no arguments at all:
调度程序的使用非常简单。要创建调度程序，用户只需提供经过时间的类型，而无需任何参数：

```C++
entt::basic_scheduler<std::uint64_t> scheduler;
```

Otherwise, the `scheduler` alias is also available for the most common cases. It uses `std::uint32_t` as a default type:
否则，"scheduler "别名也可用于最常见的情况。它使用 `std::uint32_t` 作为默认类型：

```C++
entt::scheduler scheduler;
```

The class has member functions to query its internal data structures, like `empty` or `size`, as well as a `clear` utility to reset it to a clean state:
该类有用于查询其内部数据结构（如 `empty` 或 `size` ）的成员函数，以及用于将其重置为干净状态的 `clear` 工具：

```C++
// checks if there are processes still running
const auto empty = scheduler.empty();

// gets the number of processes still running
entt::scheduler::size_type size = scheduler.size();

// resets the scheduler to its initial state and discards all the processes
scheduler.clear();
```

To attach a process to a scheduler there are mainly two ways:
将进程附加到调度程序主要有两种方法：

- If the process inherits from the `process` class template, it's enough to indicate its type and submit all the parameters required to construct it to the `attach` member function:
  如果进程继承自 `process` 类模板，则只需指明其类型并向 `attach` 成员函数提交构建进程所需的所有参数即可：

  ```C++
  scheduler.attach<my_process>(1000u);
  ```

- Otherwise, in case of a lambda or a functor, it's enough to provide an instance of the class to the `attach` member function:
  否则，如果是 lambda 或函式，只需向 `attach` 成员函数提供一个类的实例即可：

  ```C++
  scheduler.attach([](auto...){ /* ... */ });
  ```

In both cases, the scheduler is returned and its `then` member function can be used to create chains of processes to run sequentially.
在这两种情况下，调度程序都会返回，其 `then` 成员函数可用于创建按顺序运行的进程链

As a minimal example of use:

```C++
// schedules a task in the form of a lambda function
scheduler.attach([](auto delta, void *, auto succeed, auto fail) {
    // ...
})
// appends a child in the form of another lambda function
.then([](auto delta, void *, auto succeed, auto fail) {
    // ...
})
// appends a child in the form of a process class
.then<my_process>(1000u);
```

To update a scheduler and therefore all its processes, the `update` member function is the way to go:
要更新调度程序并因此更新其所有进程，可以使用 `update` 成员函数：

```C++
// updates all the processes, no user data are provided
scheduler.update(delta);

// updates all the processes and provides them with custom data
scheduler.update(delta, &data);
```

In addition to these functions, the scheduler offers an `abort` member function that is used to discard all the running processes at once:
除了这些函数，调度器还提供了一个 `abort` 成员函数，用于一次性丢弃所有正在运行的进程：

```C++
// aborts all the processes abruptly ...
scheduler.abort(true);

// ... or gracefully during the next tick
scheduler.abort();
```

# Crash Course: core functionalities

## Introduction

`EnTT` comes with a bunch of core functionalities mostly used by the other parts of the library.
Many of these tools are also useful in everyday work. Therefore, it's worth describing them so as not to reinvent the wheel in case of need.
`EnTT` 附带了一系列核心功能，这些功能主要用于库的其他部分。
其中许多工具在日常工作中也很有用。因此，为了避免在需要时重新发明轮子，值得对它们进行描述。

## Any as in any type

`EnTT` offers its own `any` type. It may seem redundant considering that C++17 introduced `std::any`, but it is not (hopefully).
EnTT提供了自己的`any`类型。考虑到 C++17 引入了 `std::any`，这似乎是多余的，但它并非如此（但愿如此）。

First of all, the *type* returned by an `std::any` is a const reference to an `std::type_info`, an implementation defined class that's not something everyone wants to see in a software. Furthermore, there is no way to bind it to the type system of the library and therefore with its integrated RTTI support.
首先，"std::any "返回的*type*是对 "std::type_info "的常量引用，而 "std::type_info "是一个实现定义的类，不是每个人都希望在软件中看到的。此外，没有办法将其与库的类型系统绑定，因此也就无法集成 RTTI 支持。

The `any` API is very similar to that of its most famous counterpart, mainly because this class serves the same purpose of being an opaque container for any type of value.
Instances also minimize the number of allocations by relying on a well known technique called *small buffer optimization* and a fake vtable.
`any` API 与它最著名的对应类非常相似，主要是因为该类的作用相同，都是作为任何类型值的不透明容器。
此外，该类实例还依赖于一种名为 "小缓冲区优化"（*small buffer optimization）的著名技术和一个假 vtable，从而最大限度地减少了分配次数。

Creating an object of the `any` type, whether empty or not, is trivial:

```C++
// an empty container
entt::any empty{};

// a container for an int
entt::any any{0};

// in place construction
entt::any in_place{std::in_place_type<int>, 42};
```

Alternatively, the `make_any` function serves the same purpose but requires to always be explicit about the type:
另外，"make_any "函数也能达到同样的目的，但要求始终明确类型：

```C++
entt::any any = entt::make_any<int>(42);
```

In both cases, the `any` class takes the burden of destroying the contained element when required, regardless of the storage strategy used for the specific object.
Furthermore, an instance of `any` isn't tied to an actual type. Therefore, the wrapper is reconfigured when it's assigned a new object of a type other than the one it contains.
在这两种情况下，`any` 类都会在需要时负责销毁所包含的元素，而不管特定对象使用的是哪种存储策略。
此外，`any` 类的实例与实际类型无关。因此，当封装器分配到一个新对象时，它将被重新配置，而不是它所包含的类型。

There is also a way to directly assign a value to the variable contained by an `entt::any`, without necessarily replacing it. This is especially useful when the object is used in *aliasing mode*, as described below:
还有一种方法可以直接为`entt::any`包含的变量赋值，而不一定要替换它。如下所述，当对象在*aliasing 模式*下使用时，这种方法尤其有用：

```C++
entt::any any{42};
entt::any value{3};

// assigns by copy
any.assign(value);

// assigns by move
any.assign(std::move(value));
```

The `any` class performs a check on the type information and whether or not the original type was copy or move assignable, as appropriate.
In all cases, the `assign` function returns a boolean value that is true in case of success and false otherwise.
任意 "类对类型信息以及原始类型是否可复制或移动分配（视情况而定）进行检查。
在所有情况下，`assign` 函数都会返回一个布尔值，成功时为 true，否则为 false。

When in doubt about the type of object contained, the `type` member function returns a const reference to the `type_info` associated with its element, or `type_id<void>()` if the container is empty.
The type is also used internally when comparing two `any` objects:
如果对所含对象的类型有疑问，`type` 成员函数会返回与其元素相关联的 `type_info` 的常量引用，如果容器为空，则返回 `type_id<void>()` 。
在比较两个 `any` 对象时，内部也会使用类型：

```C++
if(any == empty) { /* ... */ }
```

In this case, before proceeding with a comparison, it's verified that the *type* of the two objects is actually the same.
Refer to the `EnTT` type system documentation for more details about how `type_info` works and the possible risks of a comparison.
在这种情况下，在进行比较之前，要验证两个对象的 *type* 是否确实相同。
关于 `type_info` 如何工作以及比较可能带来的风险，请参阅 `EnTT` 类型系统文档。

A particularly interesting feature of this class is that it can also be used as an opaque container for const and non-const references:
该类的一个特别有趣的功能是，它还可以用作常量和非常量引用的不透明容器：

```C++
int value = 42;

entt::any any{std::in_place_type<int &>(value)};
entt::any cany = entt::make_any<const int &>(value);
entt::any fwd = entt::forward_as_any(value);

any.emplace<const int &>(value);
```

In other words, whenever `any` is explicitly told to construct an *alias*, it acts as a pointer to the original instance rather than making a copy of it or moving it internally. The contained object is never destroyed and users must ensure that its lifetime exceeds that of the container.
换句话说，只要明确告诉 `any` 构造*别名*，它就会充当指向原始实例的指针，而不是复制它或在内部移动它。所包含的对象永远不会被销毁，用户必须确保它的生命周期超过容器的生命周期。

Similarly, it's possible to create non-owning copies of `any` from an existing object:
同样，也可以从现有对象中创建 `any` 的非所有副本：

```C++
// aliasing constructor
entt::any ref = other.as_ref();
```

In this case, it doesn't matter if the original container actually holds an object or is as a reference for unmanaged elements already. The new instance thus created doesn't create copies and only serves as a reference for the original item.
在这种情况下，无论原始容器是实际持有一个对象，还是已经作为非托管元素的引用，都无关紧要。这样创建的新实例不会创建副本，只会作为原始项目的引用。

It's worth mentioning that, while everything works transparently when it comes to non-const references, there are some exceptions when it comes to const references.
In particular, the `data` member function invoked on a non-const instance of `any` that wraps a const reference returns a null pointer in all cases.
值得一提的是，虽然在涉及非 const 引用时一切都很透明，但在涉及 const 引用时却有一些例外。
特别是，在任何情况下，对封装了常量引用的 `any` 的非常量实例调用的 `data` 成员函数都会返回一个空指针。

To cast an instance of `any` to a type, the library offers a set of `any_cast` functions in all respects similar to their most famous counterparts.
The only difference is that, in the case of `EnTT`, they won't raise exceptions but will only trigger an assert in debug mode, otherwise resulting in undefined behavior in case of misuse in release mode.
要将 `any` 实例转换为类型，库提供了一组 `any_cast` 函数，这些函数在所有方面都与最著名的对应函数类似。
唯一不同的是，在 `EnTT` 的情况下，它们不会引发异常，只会在调试模式下触发断言，否则在发布模式下误用会导致未定义的行为。

### Small buffer optimization

The `any` class uses a technique called *small buffer optimization* to reduce the number of allocations where possible.
The default reserved size for an instance of `any` is `sizeof(double[2])`. However, this is also configurable if needed. In fact, `any` is defined as an alias for `basic_any<Len>`, where `Len` is the size above.
Users can easily set a custom size or define their own aliases:
`any` 类使用了一种称为*小缓冲区优化*的技术，以尽可能减少分配次数。
`any`实例的默认预留大小是 `sizeof(double[2])`。不过，如果需要，也可以进行配置。事实上，`any` 被定义为 `basic_any<Len>` 的别名，其中 `Len` 是上述大小。

用户可以轻松设置自定义大小或定义自己的别名：

```C++
using my_any = entt::basic_any<sizeof(double[4])>;
```

This feature, in addition to allowing the choice of a size that best suits the needs of an application, also offers the possibility of forcing dynamic creation of objects during construction.
In other terms, if the size is 0, `any` suppresses the small buffer optimization and always dynamically allocates objects (except for aliasing cases).
该功能除了允许选择最适合应用程序需要的大小外，还提供了在构建过程中强制动态创建对象的可能性。
换句话说，如果大小为 0，`any` 将抑制小缓冲区优化，并始终动态分配对象（别名情况除外）。

### Alignment requirement

The alignment requirement is optional and by default the most stringent (the largest) for any object whose size is at most equal to the one provided.
It's provided as an optional second parameter following the desired size for the internal storage:
对齐要求是可选的，默认情况下，对于任何大小与所提供的对齐要求相等的对象，对齐要求都是最严格的（最大的）。
对齐要求是可选的第二个参数，位于所需的内部存储空间大小之后：

```C++
using my_any = entt::basic_any<sizeof(double[4]), alignof(double[4])>;
```

The `basic_any` class template inspects the alignment requirements in each case, even when not provided and may decide not to use the small buffer optimization in order to meet them.
即使未提供对齐要求，"basic_any "类模板也会检查每种情况下的对齐要求，并可能决定不使用小缓冲区优化来满足这些要求。

## Compressed pair

Primarily designed for internal use and far from being feature complete, the `compressed_pair` class does exactly what it promises: it tries to reduce the size of a pair by exploiting *Empty Base Class Optimization* (or *EBCO*).
This class **is not** a drop-in replacement for `std::pair`. However, it offers enough functionalities to be a good alternative for when reducing memory usage is more important than having some cool and probably useless feature.
`compressed_pair`类主要是为内部使用而设计的，远非功能完备，它所做的正是它所承诺的：它试图通过利用*Empty Base Class Optimization* (或 *EBCO*)来减小pair的大小。
该类并不能直接替代 "std::pair"。不过，当减少内存使用量比拥有一些很酷但可能无用的功能更重要时，它提供的功能足以成为一个很好的替代品。

Although the API is very close to that of `std::pair` (apart from the fact that the template parameters are inferred from the constructor and therefore there is no `entt::make_compressed_pair`), the major difference is that `first` and `second` are functions for implementation requirements:
虽然 API 与 `std::pair` 非常接近（除了模板参数是从构造函数中推断出来的，因此没有 `entt::make_compressed_pair`），但主要区别在于 `first` 和 `second` 是用于实现需求的函数：

```C++
entt::compressed_pair pair{0, 3.};
pair.first() = 42;
```

There isn't much to describe then. It's recommended to rely on documentation and intuition. At the end of the day, it's just a pair and nothing more.
那就没什么好描述的了。建议依靠文件和直觉。说到底，这只是一对，仅此而已。

## Enum as bitmask

Sometimes it's useful to be able to use enums as bitmasks. However, enum classes aren't really suitable for the purpose. Main problem is that they don't convert implicitly to their underlying type.
The choice is then between using old-fashioned enums (with all their problems that I don't want to discuss here) or writing *ugly* code.
有时，将枚举用作位掩码会很有用。然而，枚举类并不适合这种用途。主要问题是它们不能隐式转换为底层类型。
因此，我们只能选择使用老式的枚举类（它们有很多问题，我不想在这里讨论），或者编写*丑陋*的代码。

Fortunately, there is also a third way: adding enough operators in the global scope to treat enum classes as bitmasks transparently.
The ultimate goal is to write code like the following (or maybe something more meaningful, but this should give a grasp and remain simple at the same time):
幸运的是，还有第三种方法：在全局范围内添加足够多的操作符，将枚举类透明地视为位掩码。
我们的最终目标是写出类似下面这样的代码（或者更有意义的代码，但这样既能让人掌握，又能保持简单）：

```C++
enum class my_flag {
    unknown = 0x01,
    enabled = 0x02,
    disabled = 0x04
};

const my_flag flags = my_flag::enabled;
const bool is_enabled = !!(flags & my_flag::enabled);
```

The problem with adding all operators to the global scope is that these come into play even when not required, with the risk of introducing errors that are difficult to deal with.
However, C++ offers enough tools to get around this problem. In particular, the library requires users to register the enum classes for which bitmask support should be enabled:
将所有操作符添加到全局作用域的问题在于，这些操作符即使在不需要时也会发挥作用，并有可能引入难以处理的错误。
不过，C++ 提供了足够的工具来解决这个问题。特别是，程序库要求用户注册应启用位掩码支持的枚举类：

```C++
template<>
struct entt::enum_as_bitmask<my_flag>
    : std::true_type
{};
```

This is handy when dealing with enum classes defined by third party libraries and over which the user has no control. However, it's also verbose and can be avoided by adding a specific value to the enum class itself:
在处理由第三方库定义的、用户无法控制的枚举类时，这很方便。不过，这种方法也很啰嗦，可以通过在枚举类中添加特定值来避免：

```C++
enum class my_flag {
    unknown = 0x01,
    enabled = 0x02,
    disabled = 0x04,
    _entt_enum_as_bitmask
};
```

In this case, there is no need to specialize the `enum_as_bitmask` traits, since `EnTT` automatically detects the flag and enables the bitmask support.
Once the enum class is registered (in one way or the other), the most common operators such as `&`, `|` but also `&=` and `|=` are available for use.
在这种情况下，不需要专门化 `enum_as_bitmask` 特质，因为 `EnTT` 会自动检测标志并启用位掩码支持。
一旦注册了枚举类（以这种或那种方式），就可以使用最常用的运算符，如 `&`、`|` 以及 `&=` 和 `|=`。

Refer to the official documentation for the full list of operators.
有关操作员的完整列表，请参阅官方文档。

## Hashed strings

Hashed strings are human-readable identifiers in the codebase that turn into numeric values at runtime, thus without affecting performance.
The class has an implicit `constexpr` constructor that chews a bunch of characters. Once created, one can get the original string by means of the `data` member function or convert the instance into a number.
A hashed string is well suited wherever a constant expression is required. No *string-to-number* conversion will take place at runtime if used carefully.
散列字符串是代码库中人类可读的标识符，在运行时会变成数值，因此不会影响性能。
该类有一个隐式的 `constexpr` 构造函数，可以嚼碎一堆字符。创建后，可以通过 `data` 成员函数获取原始字符串，或将实例转换为数字。
散列字符串非常适合需要常量表达式的地方。如果使用得当，运行时不会发生字符串到数字的转换。

Example of use:

```C++
auto load(entt::hashed_string::hash_type resource) {
    // uses the numeric representation of the resource to load and return it
}

auto resource = load(entt::hashed_string{"gui/background"});
```

There is also a *user defined literal* dedicated to hashed strings to make them more *user-friendly*:
还有一个*用户定义的字面*专门用于散列字符串，使其更加*用户友好*：

```C++
using namespace entt::literals;
constexpr auto str = "text"_hs;
```

User defined literals in `EnTT` are enclosed in the `entt::literals` namespace. Therefore, the entire namespace or selectively the literal of interest must be explicitly included before each use, a bit like `std::literals`.
在 `EnTT` 中，用户定义的字面量被包含在 `entt::literals` 命名空间中。因此，在每次使用前，必须显式地包含整个命名空间或选择性地包含感兴趣的字面量，有点像 `std::litals`。

The class also offers the necessary functionalities to create hashed strings at runtime:
该类还提供在运行时创建散列字符串的必要功能：

```C++
std::string orig{"text"};

// create a full-featured hashed string...
entt::hashed_string str{orig.c_str()};

// ... or compute only the unique identifier
const auto hash = entt::hashed_string::value(orig.c_str());
```

This possibility shouldn't be exploited in tight loops, since the computation takes place at runtime and no longer at compile-time. It could therefore affect performance to some degrees.
这种可能性不应在紧密循环中利用，因为计算是在运行时进行的，而不再是编译时。因此，这可能会在一定程度上影响性能。

### Wide characters

The `hashed_string` class is an alias for `basic_hashed_string<char>`. To use the C++ type for wide character representations, there exists also the alias `hashed_wstring` for `basic_hashed_string<wchar_t>`.
In this case, the user defined literal to use to create hashed strings on the fly is `_hws`:
`hashed_string `类是 `basic_hashed_string<char>`的别名。如果要将 C++ 类型用于宽字符表示，还可以使用`hashed_wstring`作为`basic_hashed_string<wchar_t>`的别名。
在这种情况下，用户定义的用于即时创建散列字符串的文字是 `_hws`：

```C++
constexpr auto str = L"text"_hws;
```

The hash type of `hashed_wstring` is the same as its counterpart.

### Conflicts

The hashed string class uses FNV-1a internally to hash strings. Because of the *pigeonhole principle*, conflicts are possible. This is a fact.
There is no silver bullet to solve the problem of conflicts when dealing with hashing functions. In this case, the best solution is likely to give up. That's all.
After all, human-readable unique identifiers aren't something strictly defined and over which users have not the control. Choosing a slightly different identifier is probably the best solution to make the conflict disappear in this case.
散列字符串类内部使用 FNV-1a 对字符串进行散列。由于*鸽孔原理*，冲突是可能发生的。这是事实。
在处理散列函数时，没有解决冲突问题的灵丹妙药。在这种情况下，最好的解决办法可能就是放弃。仅此而已。
毕竟，人类可读的唯一标识符不是严格定义的，用户无法控制。在这种情况下，选择一个稍有不同的标识符可能是使冲突消失的最佳解决方案。

## Iterators

Writing and working with iterators isn't always easy. More often than not it also leads to duplicated code.
`EnTT` tries to overcome this problem by offering some utilities designed to make this hard work easier.
编写和使用迭代器并不总是那么容易。通常还会导致代码重复。
为了克服这个问题，`EnTT` 提供了一些实用工具，旨在让这项艰巨的工作变得更轻松。

### Input iterator pointer

When writing an input iterator that returns in-place constructed values if dereferenced, it's not always straightforward to figure out what `value_type` is and how to make it behave like a full-fledged pointer.
Conversely, it would be very useful to have an `operator->` available on the iterator itself that always works without too much complexity.
在编写输入迭代器（如果取消引用，则返回就地构造的值）时，要弄清 `value_type` 是什么以及如何使其表现得像一个正式的指针并不总是那么简单。
相反，如果能在迭代器上提供一个 `operator->` 就会非常有用，因为它总能在不太复杂的情况下工作。

The input iterator pointer is meant for this. It's a small class that wraps the in-place constructed value and adds some functions on top of it to make it suitable for use with input iterators:
输入迭代器指针就是为此而设计的。它是一个小类，封装了就地构造值，并在其上添加了一些函数，使其适用于输入迭代器：

```C++
struct iterator_type {
    using value_type = std::pair<first_type, second_type>;
    using pointer = input_iterator_pointer<value_type>;
    using reference = value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    // ...
}
```

The library makes extensive use of this class internally. In many cases, the `value_type` of the returned iterators is just an input iterator pointer.
库在内部广泛使用该类。在许多情况下，返回迭代器的 `value_type` 只是一个输入迭代器指针。

### Iota iterator

Waiting for C++20, this iterator accepts an integral value and returns all elements in a certain range:
在 C++20 中，该迭代器接受一个积分值，并返回一定范围内的所有元素：

```
entt::iota_iterator first{0};
entt::iota_iterator last{100};

for(; first != last; ++first) {
    int value = *first;
    // ...
}
```

In the future, views will replace this class. Meanwhile, the library makes some interesting uses of it when a range of integral values is to be returned to the user.
未来，视图将取代该类。同时，当需要向用户返回一定范围的积分值时，程序库会对它进行一些有趣的使用。

### Iterable adaptor

Typically, a container class provides `begin` and `end` member functions (with their const counterparts) for iteration.
However, it can happen that a class offers multiple iteration methods or allows users to iterate different sets of *elements*.
通常，容器类提供用于迭代的 `begin` 和 `end` 成员函数（及其常量对应函数）。
然而，一个类可能会提供多种迭代方法，或允许用户迭代不同的 * 元素集。

The iterable adaptor is a utility class that makes it easier to use and access data in this case.
It accepts a couple of iterators (or an iterator and a sentinel) and offers an *iterable* object with all the expected methods like `begin`, `end` and whatnot.
迭代适配器是一个实用类，在这种情况下可以更方便地使用和访问数据。
它接受几个迭代器（或一个迭代器和一个哨兵），并提供一个 *iterable*对象，其中包含所有预期的方法，如`begin`、`end`等。

The library uses this class extensively.
Think for example of views, which can be iterated to access entities but also offer a method of obtaining an iterable object that returns tuples of entities and components at once.
Another example is the registry class which allows users to iterate its storage by returning an iterable object for the purpose.
该库广泛使用该类。
例如，视图可以通过迭代来访问实体，但同时也提供了一种获取可迭代对象的方法，可以一次性返回实体和组件的元组。
另一个例子是注册表类，它允许用户通过返回一个可迭代对象来迭代其存储空间。

## Memory

There are a handful of tools within `EnTT` to interact with memory in one way or another.
Some are geared towards simplifying the implementation of (internal or external) allocator aware containers. Others are designed to help the developer with everyday problems.
在 `EnTT`中，有一些工具可以以这样或那样的方式与内存交互。
有些工具旨在简化（内部或外部）分配器感知容器的实现。其他工具则旨在帮助开发人员解决日常问题。

The former are very specific and for niche problems. These are tools designed to unwrap fancy or plain pointers (`to_address`) or to help forget the meaning of acronyms like *POCCA*, *POCMA* or *POCS*.
I won't describe them here in detail. Instead, I recommend reading the inline documentation to those interested in the subject.
前者非常具体，用于解决小众问题。这些工具的目的是解开花哨或简单的指针（`to_address`），或帮助忘记 *POCCA*、*POCMA* 或 *POCS* 等缩写词的含义。
在此我就不详细介绍了。相反，我建议对该主题感兴趣的人阅读内联文档。

### Power of two and fast modulus

Finding out if a number is a power of two (`is_power_of_two`) or what the next power of two is given a random value (`next_power_of_two`) is very useful at times.
For example, it helps to allocate memory in pages having a size suitable for the fast modulus:
查找一个数字是否是二的幂次（`is_power_of_two`）或给定一个随机值的下一个二的幂次（`next_power_of_two`）有时非常有用。
例如，它有助于在大小适合快速模数的页面中分配内存：

```
const std::size_t result = entt::fast_mod(value, modulus);
```

Where `modulus` is necessarily a power of two. Perhaps not everyone knows that this type of operation is far superior in terms of performance to the basic modulus and for this reason preferred in many areas.
其中，"模数 "必须是 2 的幂次。也许不是每个人都知道，这种运算方式在性能上远远优于基本模数运算，因此在许多领域更受青睐。

### Allocator aware unique pointers

A nasty thing in C++ (at least up to C++20) is the fact that shared pointers support allocators while unique pointers don't.
There is a proposal at the moment that also shows (among the other things) how this can be implemented without any compiler support.
在 C++ 中（至少在 C++20 之前），共享指针支持分配器，而唯一指针不支持分配器，这是一个令人讨厌的问题。
目前有一项提案也展示了（除其他外）如何在没有任何编译器支持的情况下实现这一点。

The `allocate_unique` function follows this proposal, making a virtue out of necessity:
`allocate_unique `函数沿用了这一建议，将必要性变成了优点：

```C++
std::unique_ptr<my_type, entt::allocation_deleter<my_type>> ptr = entt::allocate_unique<my_type>(allocator, arguments);
```

Although the internal implementation is slightly different from what is proposed for the standard, this function offers an API that is a drop-in replacement for the same feature.
虽然内部实现与标准建议的略有不同，但该功能提供的应用程序接口可以直接替代相同的功能。

## Monostate

The monostate pattern is often presented as an alternative to a singleton based configuration system.
This is exactly its purpose in `EnTT`. Moreover, this implementation is thread safe by design (hopefully).
单态模式通常被用作基于单例的配置系统的替代方案。
这正是它在 `EnTT`中的用途。此外，这种实现在设计上是线程安全的（希望如此）。

Keys are integral values (easily obtained by hashed strings), values are basic types like `int`s or `bool`s. Values of different types can be associated with each key, even more than one at a time.
Because of this, one should pay attention to use the same type both during an assignment and when trying to read back the data. Otherwise, there is the risk to incur in unexpected results.
键是积分值（可通过散列字符串轻松获取），值是基本类型，如 "int "或 "bool"。每个键可以关联不同类型的值，甚至一次可以关联多个值。
因此，在赋值和回读数据时都应注意使用相同的类型。否则，有可能产生意想不到的结果。

Example of use:

```C++
entt::monostate<entt::hashed_string{"mykey"}>{} = true;
entt::monostate<"mykey"_hs>{} = 42;

// ...

const bool b = entt::monostate<"mykey"_hs>{};
const int i = entt::monostate<entt::hashed_string{"mykey"}>{};
```

## Type support

`EnTT` provides some basic information about types of all kinds.
It also offers additional features that are not yet available in the standard library or that will never be.
`EnTT` 提供了关于各种类型的一些基本信息。
它还提供了标准库中尚未提供或永远不会提供的额外功能。

### Built-in RTTI support

Runtime type identification support (or RTTI) is one of the most frequently disabled features in the C++ world, especially in the gaming sector. Regardless of the reasons for this, it's often a shame not to be able to rely on opaque type information at runtime.
The library tries to fill this gap by offering a built-in system that doesn't serve as a replacement but comes very close to being one and offers similar information to that provided by its counterpart.
运行时类型标识支持（或 RTTI）是 C++ 世界中最常被禁用的功能之一，尤其是在游戏领域。无论原因如何，无法在运行时依赖不透明的类型信息往往是一种遗憾。
该库试图通过提供一个内置系统来填补这一空白，该系统虽然不能替代类型信息，但却非常接近于类型信息，并能提供与其对应系统类似的信息。

Basically, the whole system relies on a handful of classes. In particular:
基本上，整个系统都依赖于少数几个类。特别是

- The unique sequential identifier associated with a given type:
  与给定类型相关的唯一顺序标识符：

  ```C++
  auto index = entt::type_index<a_type>::value();
  ```

  

  The returned value isn't guaranteed to be stable across different runs.
  However, it can be very useful as index in associative and unordered associative containers or for positional accesses in a vector or an array.
  在不同的运行中，返回值并不保证稳定。
  不过，它可以作为关联和无序关联容器的索引，或用于向量或数组中的位置访问，非常有用。

  An external generator can also be used if needed. In fact, `type_index` can be specialized by type and is also *sfinae-friendly* in order to allow more refined specializations such as:
  如果需要，也可以使用外部生成器。事实上，type_index 可以按类型进行特殊化，而且对 sfinae 也很友好，以便进行更精细的特殊化，例如：type_index 可以按类型进行特殊化，而且对 sfinae 也很友好，以便进行更精细的特殊化：

  ```C++
  template<typename Type>
  struct entt::type_index<Type, std::void_d<decltype(Type::index())>> {
      static entt::id_type value() noexcept {
          return Type::index();
      }
  };
  ```

  Indexes **must** be sequentially generated in this case.
  The tool is widely used within `EnTT`. Generating indices not sequentially would break an assumption and would likely lead to undesired behaviors.
  在这种情况下，索引***必须按顺序生成。
  该工具在 "EnTT "中被广泛使用。不按顺序生成索引会破坏一个假设，并可能导致不希望出现的行为。

- The hash value associated with a given type:
  与给定类型相关的哈希值：

  ```C++
  auto hash = entt::type_hash<a_type>::value();
  ```

  In general, the `value` function exposed by `type_hash` is also `constexpr` but this isn't guaranteed for all compilers and platforms (although it's valid with the most well-known and popular ones).
  一般来说，`type_hash` 揭示的 `value` 函数也是 `constexpr`，但这并不能保证适用于所有编译器和平台（尽管它适用于最著名、最流行的编译器和平台）。

  This function **can** use non-standard features of the language for its own purposes. This makes it possible to provide compile-time identifiers that remain stable across different runs.
  Users can prevent the library from using these features by means of the `ENTT_STANDARD_CPP` definition. In this case, there is no guarantee that identifiers remain stable across executions. Moreover, they are generated at runtime and are no longer a compile-time thing.
  该函数***可以为自己的目的使用语言的非标准特性。这使得提供在不同运行中保持稳定的编译时标识符成为可能。
  用户可以通过 `ENTT_STANDARD_CPP` 定义阻止库使用这些特性。在这种情况下，无法保证标识符在不同执行过程中保持稳定。此外，标识符是在运行时生成的，不再是编译时的东西。

  As it happens with `type_index`, also `type_hash` is a *sfinae-friendly* class that can be specialized in order to customize its behavior globally or on a per-type or per-traits basis.
  和 `type_index` 一样，`type_hash` 也是一个*sfinae*友好的类，它可以被特殊化，以便在全局或每个类型或每个属性的基础上自定义其行为。

- The name associated with a given type:
  与给定类型相关的名称：

  ```C++
  auto name = entt::type_name<a_type>::value();
  ```

  This value is extracted from some information generally made available by the compiler in use. Therefore, it may differ depending on the compiler and may be empty in the event that this information isn't available.
  该值是从所使用的编译器通常提供的一些信息中提取的。因此，该值可能因编译器而异，如果没有这些信息，则可能为空。

  For example, given the following class:

  ```C++
  struct my_type { /* ... */ };
  ```

  The name is `my_type` when compiled with GCC or CLang and `struct my_type` when MSVC is in use.
  Most of the time the name is also retrieved at compile-time and is therefore always returned through an `std::string_view`. Users can easily access it and modify it as needed, for example by removing the word `struct` to normalize the result. `EnTT` doesn't do this for obvious reasons, since it would be creating a new string at runtime otherwise.
  使用 GCC 或 CLang 编译时，名称为 `my_type`；使用 MSVC 时，名称为 `struct my_type`。
  大多数情况下，该名称也是在编译时获取的，因此总是通过 `std::string_view`返回。用户可以很容易地访问它，并根据需要对其进行修改，例如删除 `struct` 字样以规范化结果。出于显而易见的原因，`EnTT` 不会这样做，否则它将在运行时创建一个新字符串。

  This function **can** use non-standard features of the language for its own purposes. Users can prevent the library from using these features by means of the `ENTT_STANDARD_CPP` definition. In this case, the name is just empty.
  该函数可以为自己的目的使用语言的非标准特性。用户可以通过 `ENTT_STANDARD_CPP` 定义阻止库使用这些功能。在这种情况下，名称为空。

  As it happens with `type_index`, also `type_name` is a *sfinae-friendly* class that can be specialized in order to customize its behavior globally or on a per-type or per-traits basis.
  与 `type_index` 一样，`type_name` 也是一个*sfinae*友好的类，它可以被特殊化，以便在全局或每个类型或每个属性的基础上自定义其行为。

These are then combined into utilities that aim to offer an API that is somewhat similar to that made available by the standard library.
然后，这些程序被组合成实用程序，旨在提供与标准程序库类似的应用程序接口。

#### Type info

The `type_info` class isn't a drop-in replacement for `std::type_info` but can provide similar information which are not implementation defined and don't require to enable RTTI.
Therefore, they can sometimes be even more reliable than those obtained otherwise.
`type_info`类并不能直接替代 `std::type_info`，但可以提供类似的信息，这些信息没有实现定义，也不需要启用 RTTI。
因此，它们有时甚至比其他方法获得的信息更可靠。

Its type defines an opaque class that is also copyable and movable.
Objects of this type are generally returned by the `type_id` functions:
它的类型定义了一个不透明的类，也是可复制和可移动的。
这种类型的对象通常由 `type_id` 函数返回：

```C++
// by type
auto info = entt::type_id<a_type>();

// by value
auto other = entt::type_id(42);
```

All elements thus received are nothing more than const references to instances of `type_info` with static storage duration.
This is convenient for saving the entire object aside for the cost of a pointer. However, nothing prevents from constructing `type_info` objects directly:
这样接收到的所有元素都不过是对具有静态存储期限的 `type_info` 实例的常量引用。
这样就可以方便地保存整个对象，而不需要支付指针的费用。然而，没有什么能阻止直接构造 `type_info` 对象：

```C++
entt::type_info info{std::in_place_type<int>};
```

These are the information made available by `type_info`:

- The index associated with a given type:

  ```
  auto idx = entt::type_id<a_type>().index();
  ```

  

  This is also an alias for the following:

  ```
  auto idx = entt::type_index<std::remove_cv_t<std::remove_reference_t<a_type>>>::value();
  ```

  

- The hash value associated with a given type:

  ```
  auto hash = entt::type_id<a_type>().hash();
  ```

  

  This is also an alias for the following:

  ```
  auto hash = entt::type_hash<std::remove_cv_t<std::remove_reference_t<a_type>>>::value();
  ```

  

- The name associated with a given type:

  ```
  auto name = entt::type_id<my_type>().name();
  ```

  

  This is also an alias for the following:

  ```
  auto name = entt::type_name<std::remove_cv_t<std::remove_reference_t<a_type>>>::value();
  ```

  

Where all accessed features are available at compile-time, the `type_info` class is also fully `constexpr`. However, this cannot be guaranteed in advance and depends mainly on the compiler in use and any specializations of the classes described above.

### Almost unique identifiers



Since the default non-standard, compile-time implementation of `type_hash` makes use of hashed strings, it may happen that two types are assigned the same hash value.
In fact, although this is quite rare, it's not entirely excluded.

Another case where two types are assigned the same identifier is when classes from different contexts (for example two or more libraries loaded at runtime) have the same fully qualified name. In this case, `type_name` returns the same value for the two types.
Fortunately, there are several easy ways to deal with this:

- The most trivial one is to define the `ENTT_STANDARD_CPP` macro. Runtime identifiers don't suffer from the same problem in fact. However, this solution doesn't work well with a plugin system, where the libraries aren't linked.
- Another possibility is to specialize the `type_name` class for one of the conflicting types, in order to assign it a custom identifier. This is probably the easiest solution that also preserves the feature of the tool.
- A fully customized identifier generation policy (based for example on enum classes or preprocessing steps) may represent yet another option.

These are just some examples of possible approaches to the problem but there are many others. As already mentioned above, since users have full control over their types, this problem is in any case easy to solve and should not worry too much.
In all likelihood, it will never happen to run into a conflict anyway.

## Type traits



A handful of utilities and traits not present in the standard template library but which can be useful in everyday life.
This list **is not** exhaustive and contains only some of the most useful classes. Refer to the inline documentation for more information on the features offered by this module.

### Size of



The standard operator `sizeof` complains if users provide it with functions or incomplete types. On the other hand, it's guaranteed that its result is always non-zero, even if applied to an empty class type.
This small class combines the two and offers an alternative to `sizeof` that works under all circumstances, returning zero if the type isn't supported:

```
const auto size = entt::size_of_v<void>;
```



### Is applicable



The standard library offers the great `std::is_invocable` trait in several forms. This takes a function type and a series of arguments and returns true if the condition is satisfied.
Moreover, users are also provided with `std::apply`, a tool for combining invocable elements and tuples of arguments.

It would therefore be a good idea to have a variant of `std::is_invocable` that also accepts its arguments in the form of a tuple-like type, so as to complete the offer:

```
constexpr bool result = entt::is_applicable<Func, std::tuple<a_type, another_type>>;
```



This trait is built on top of `std::is_invocable` and does nothing but unpack a tuple-like type and simplify the code at the call site.

### Constness as



A utility to easily transfer the constness of a type to another type:

```
// type is const dst_type because of the constness of src_type
using type = entt::constness_as_t<dst_type, const src_type>;
```



The trait is subject to the rules of the language. For example, *transferring* constness between references won't give the desired effect.

### Member class type



The `auto` template parameter introduced with C++17 made it possible to simplify many class templates and template functions but also made the class type opaque when members are passed as template arguments.
The purpose of this utility is to extract the class type in a few lines of code:

```
template<typename Member>
using clazz = entt::member_class_t<Member>;
```



### N-th argument



A utility to quickly find the n-th argument of a function, member function or data member (for blind operations on opaque types):

```
using type = entt::nth_argument_t<1u, decltype(&clazz::member)>;
```



Disambiguation of overloaded functions is the responsibility of the user, should it be needed.

### Integral constant



Since `std::integral_constant` may be annoying because of its form that requires to specify both a type and a value of that type, there is a more user-friendly shortcut for the creation of integral constants.
This shortcut is the alias template `entt::integral_constant`:

```
constexpr auto constant = entt::integral_constant<42>;
```



Among the other uses, when combined with a hashed string it helps to define tags as human-readable *names* where actual types would be required otherwise:

```
constexpr auto enemy_tag = entt::integral_constant<"enemy"_hs>;
registry.emplace<enemy_tag>(entity);
```



### Tag



Type `id_type` is very important and widely used in `EnTT`. Therefore, there is a more user-friendly shortcut for the creation of constants based on it.
This shortcut is the alias template `entt::tag`.

If used in combination with hashed strings, it helps to use human-readable names where types would be required otherwise. As an example:

```
registry.emplace<entt::tag<"enemy"_hs>>(entity);
```



However, this isn't the only permitted use. Literally any value convertible to `id_type` is a good candidate, such as the named constants of an unscoped enum.

### Type list and value list



There is no respectable library where the much desired *type list* can be missing.
`EnTT` is no exception and provides (making extensive use of it internally) the `type_list` type, in addition to its `value_list` counterpart dedicated to non-type template parameters.

Here is a (possibly incomplete) list of the functionalities that come with a type list:

- `type_list_element[_t]` to get the N-th element of a type list.
- `type_list_index[_v]` to get the index of a given element of a type list.
- `type_list_cat[_t]` and a handy `operator+` to concatenate type lists.
- `type_list_unique[_t]` to remove duplicate types from a type list.
- `type_list_contains[_v]` to know if a type list contains a given type.
- `type_list_diff[_t]` to remove types from type lists.
- `type_list_transform[_t]` to *transform* a range and create another type list.

I'm also pretty sure that more and more utilities will be added over time as needs become apparent.
Many of these functionalities also exist in their version dedicated to value lists. We therefore have `value_list_element[_v]` as well as `value_list_cat[_t]`and so on.

# Unique sequential identifiers



Sometimes it's useful to be able to give unique, sequential numeric identifiers to types either at compile-time or runtime.
There are plenty of different solutions for this out there and I could have used one of them. However, I decided to spend my time to define a couple of tools that fully embraces what the modern C++ has to offer.

## Compile-time generator



To generate sequential numeric identifiers at compile-time, `EnTT` offers the `ident` class template:

```
// defines the identifiers for the given types
using id = entt::ident<a_type, another_type>;

// ...

switch(a_type_identifier) {
case id::value<a_type>:
    // ...
    break;
case id::value<another_type>:
    // ...
    break;
default:
    // ...
}
```



This is what this class template has to offer: a `value` inline variable that contains a numeric identifier for the given type. It can be used in any context where constant expressions are required.

As long as the list remains unchanged, identifiers are also guaranteed to be stable across different runs. In case they have been used in a production environment and a type has to be removed, one can just use a placeholder to leave the other identifiers unchanged:

```
template<typename> struct ignore_type {};

using id = entt::ident<
    a_type_still_valid,
    ignore_type<no_longer_valid_type>,
    another_type_still_valid
>;
```



Perhaps a bit ugly to see in a codebase but it gets the job done at least.

## Runtime generator



The `family` class template helps to generate sequential numeric identifiers for types at runtime:

```
// defines a custom generator
using id = entt::family<struct my_tag>;

// ...

const auto a_type_id = id::value<a_type>;
const auto another_type_id = id::value<another_type>;
```



This is what a *family* has to offer: a `value` inline variable that contains a numeric identifier for the given type.
The generator is customizable, so as to get different *sequences* for different purposes if needed.

Identifiers aren't guaranteed to be stable across different runs. Indeed it mostly depends on the flow of execution.

# Utilities



It's not possible to escape the temptation to add utilities of some kind to a library. In fact, `EnTT` also provides a handful of tools to simplify the life of developers:

- `entt::identity`: the identity function object that will be available with C++20. It returns its argument unchanged and nothing more. It's useful as a sort of *do nothing* function in template programming.

- `entt::overload`: a tool to disambiguate different overloads from their function type. It works with both free and member functions.
  Consider the following definition:

  ```
  struct clazz {
      void bar(int) {}
      void bar() {}
  };
  ```

  

  This utility can be used to get the *right* overload as:

  ```
  auto *member = entt::overload<void(int)>(&clazz::bar);
  ```

  

  The line above is literally equivalent to:

  ```
  auto *member = static_cast<void(clazz:: *)(int)>(&clazz::bar);
  ```

  

  Just easier to read and shorter to type.

- `entt::overloaded`: a small class template used to create a new type with an overloaded `operator()` from a bunch of lambdas or functors.
  As an example:

  ```
  entt::overloaded func{
      [](int value) { /* ... */ },
      [](char value) { /* ... */ }
  };
  
  func(42);
  func('c');
  ```

  

  Rather useful when doing metaprogramming and having to pass to a function a callable object that supports multiple types at once.

- `entt::y_combinator`: this is a C++ implementation of **the** *y-combinator*. If it's not clear what it is, there is probably no need for this utility.
  Below is a small example to show its use:

  ```
  entt::y_combinator gauss([](const auto &self, auto value) -> unsigned int {
      return value ? (value + self(value-1u)) : 0;
  });
  
  const auto result = gauss(3u);
  ```

  

  Maybe convoluted at a first glance but certainly effective. Unfortunately, the language doesn't make it possible to do much better.

This is a rundown of the (actually few) utilities made available by `EnTT`. The list will probably grow over time but the size of each will remain rather small, as has been the case so far.