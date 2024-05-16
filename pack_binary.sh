#!/bin/bash

APP_PATH="./build/matchwatcher"

DEPLOY_DIR="deploy"

rm -rf $DEPLOY_DIR
mkdir -p $DEPLOY_DIR/lib
mkdir -p $DEPLOY_DIR/plugins/platforms

for lib in $(ldd $APP_PATH | grep "=> /" | awk '{print $3}'); do
    cp "$lib" $DEPLOY_DIR/lib/
done

QT_PLUGINS_PATH=$(find /usr -name "libqxcb.so" 2>/dev/null)
if [ -z "$QT_PLUGINS_PATH" ]; then
    echo "libqxcb.so not found."
    exit 1
fi
QT_PLUGINS_DIR=$(dirname $QT_PLUGINS_PATH)
cp $QT_PLUGINS_DIR/*.so $DEPLOY_DIR/plugins/platforms/


cp /usr/lib/x86_64-linux-gnu/libX11-xcb.so.1 $DEPLOY_DIR/lib/
cp /usr/lib/x86_64-linux-gnu/libxcb.so.1 $DEPLOY_DIR/lib/
cp /usr/lib/x86_64-linux-gnu/libxcb-render.so.0 $DEPLOY_DIR/lib/
cp /usr/lib/x86_64-linux-gnu/libxcb-shape.so.0 $DEPLOY_DIR/lib/
cp /usr/lib/x86_64-linux-gnu/libxcb-shm.so.0 $DEPLOY_DIR/lib/
cp /usr/lib/x86_64-linux-gnu/libxcb-xfixes.so.0 $DEPLOY_DIR/lib/

for lib in $(ldd $QT_PLUGINS_PATH | grep "=> /" | awk '{print $3}'); do
    cp "$lib" $DEPLOY_DIR/lib/
done

cp $APP_PATH $DEPLOY_DIR/

cat <<EOL > $DEPLOY_DIR/run_app.sh
#!/bin/bash
DIR=\$(dirname \$0)
export LD_LIBRARY_PATH=\$DIR/lib
export QT_QPA_PLATFORM_PLUGIN_PATH=\$DIR/plugins/platforms
\$DIR/$(basename $APP_PATH) "\$@"
EOL

chmod +x $DEPLOY_DIR/run_app.sh

