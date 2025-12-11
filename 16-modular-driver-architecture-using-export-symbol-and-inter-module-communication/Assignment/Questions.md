# Assignments

## 1. Concept Check

- What is EXPORT_SYMBOL() used for?
- Why does loading order matter?
- What happens if a module uses a symbol before it's exported?
- How is device_driver.ko logically dependent on core_driver.ko?
- Can you export data structures too?

    ## 2. Add Version Function

    In core_driver.c, add:

    ```c
    const char *core_version(void) {
        return "CoreDriver v1.0";
    }
    EXPORT_SYMBOL(core_version);
    ```

    In device_driver.c, call it on open() and print:

    ```
    [device] using core version: CoreDriver v1.0
    ```

## 3. Fail Gracefully

Try loading device_driver.ko without loading core_driver.ko first. Observe the symbol error:

```
insmod: ERROR: could not insert module: Unknown symbol in module
```