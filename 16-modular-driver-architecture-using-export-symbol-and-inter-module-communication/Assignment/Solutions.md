## 1. Concept Check

### Answers

- **What is EXPORT_SYMBOL() used for?**
    Makes kernel symbols (functions, variables) accessible to other loadable modules.

- **Why does loading order matter?**
    Dependent modules must load after modules that export required symbols. Loading order determines symbol availability.

- **What happens if a module uses a symbol before it's exported?**
    Module loading fails with an "Unknown symbol" error during insmod/modprobe.

- **How is device_driver.ko logically dependent on core_driver.ko?**
    device_driver.ko imports symbols exported by core_driver.ko via EXPORT_SYMBOL(). It requires core_driver.ko to be loaded first.

- **Can you export data structures too?**
    Yes, any kernel symbol (functions, variables, structures) can be exported using EXPORT_SYMBOL().