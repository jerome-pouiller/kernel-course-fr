dd if=/dev/test bs=1 count=1 &
echo -n t > /dev/test
echo -n t > /dev/test
dd if=/dev/test bs=1 count=1
cat /dev/test &
echo -n t > /dev/test
echo -n t > /dev/test
echo -n t > /dev/test


