## Build and Test

```bash
make
sudo insmod multi_dev.ko
```

### Check created devices:
```bash
ls -l /dev/mydev*
```

### Test each:
```bash
echo "A0" > /dev/mydev0
echo "A1" > /dev/mydev1
cat /dev/mydev0
cat /dev/mydev1
```

### Check logs:
```bash
dmesg | tail -10
```

## Cleanup

```bash
sudo rmmod multi_dev.ko
sudo rm /dev/mydev*
```
