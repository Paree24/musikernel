PYDAW_VERSION=pydaw4

if [ -f $PYDAW_VERSION.ova ]; then
	rm -f $PYDAW_VERSION.ova
fi

vboxmanage export $PYDAW_VERSION -o $PYDAW_VERSION.ova

