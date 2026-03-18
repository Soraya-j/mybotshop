#!/usr/bin/env bash

# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2025, MYBOTSHOP GmbH, Inc., All rights reserved.

# ----------------------------------------------
# Configuration Variables
# ----------------------------------------------
SERVER_IP="192.168.123.18"
DOMAIN_NAME="control.mybotshop.de"
NGINX_CONF="/etc/nginx/sites-available/go2_webserver"

SSL_DIR="/etc/nginx/ssl"
PROXY_PASS_URL="http://127.0.0.1:9000"
HOSTS_FILE="/etc/hosts"  

# Define color codes
ORANGE='\033[38;5;208m'
YELLOW='\033[93m'
WHITE='\033[0m'
NC='\033[0m' 
log() { echo -e "${1}${2}${NC}"; }

# ----------------------------------------------
# Web Server Dependencies Installation
# ----------------------------------------------
log "${ORANGE}" "Web Server Dependencies Installation"
sudo apt update
sudo apt install -y alsa-utils tigervnc-standalone-server xfce4 xfce4-goodies dbus-x11 websockify ffmpeg python3-pyaudio python3-pip

log "${ORANGE}" "Installing Webserver Python3 dependencies..."
python3 -m pip install playsound Flask waitress edge-tts pydub SpeechRecognition

log "${ORANGE}" "ROS2 | Image Transport installation"
sudo apt install ros-${ROS_DISTRO}-rosbridge-server ros-${ROS_DISTRO}-image-transport*

log "${ORANGE}" "Updating ufw policies to allow port 5901 (VNC), 6080 (noVNC), and 9000 (Webserver)"
sudo ufw allow 5901/tcp && sudo ufw allow 6080/tcp && sudo ufw allow 9000 && sudo ufw allow 80 &&sudo ufw allow 443

# ----------------------------------------------
# VNC Server Configuration
# ----------------------------------------------
mkdir -p ~/.vnc
echo "mybotshop" | vncpasswd -f > ~/.vnc/passwd
chmod 600 ~/.vnc/passwd

cat > ~/.vnc/xstartup << 'EOF'
#!/bin/bash
unset SESSION_MANAGER
unset DBUS_SESSION_BUS_ADDRESS
export XKL_XMODMAP_DISABLE=1

# Force X11 (avoid Wayland) and software rendering Ignore Nvidia issues
export XDG_SESSION_TYPE=x11
unset WAYLAND_DISPLAY
export GDK_BACKEND=x11
export QT_QPA_PLATFORM=xcb
export LIBGL_ALWAYS_SOFTWARE=1
export DISPLAY=${DISPLAY:-:1}

# Start basic X services
xsetroot -solid grey
vncconfig -iconic &

# Start XFCE (keep running in background)
startxfce4 &

# Requires Superuser privileges to run
# sudo chmod +x .vnc/xstartup
# Wait a bit for XFCE to load before setting the wallpaper
source /opt/mybotshop/install/setup.bash
(sleep 5 && \
 WALLPAPER_PATH="$(ros2 pkg prefix go2_webserver)/share/go2_webserver/go2_webserver/static/media/wallpaper/vnc_wallpaper.png" && \
 for i in {0..3}; do
   xfconf-query -c xfce4-desktop \
     -p /backdrop/screen0/monitorVNC-0/workspace$i/last-image \
     -n -t string -s "$WALLPAPER_PATH"
   xfconf-query -c xfce4-desktop \
     -p /backdrop/screen0/monitorVNC-0/workspace$i/image-style \
     -n -t int -s 3
 done && xfdesktop --reload) &

# Keep the session alive
wait
EOF

sudo chmod +x ~/.vnc/xstartup

# --- Security Error in Unitree Robots/ Nvidia Platforms

# --- Check without password: 
#     vncserver :1 -geometry 1280x800 -localhost no -SecurityTypes None --I-KNOW-THIS-IS-INSECURE

# --- Reinsert password: 
#     vncpasswd ~/.vnc/passwd
#     Ensure view only is enabled with same password
log ""
log "${YELLOW}" "To start the VNC server:"
log "${WHITE}" "vncserver :1 -geometry 1920x1080 -depth 24 -localhost no"
log ""
log "${YELLOW}" "To check VNC server status:"
log "${WHITE}" "vncserver -list"
log ""
log "${YELLOW}" "To stop the VNC server:"
log "${WHITE}" "vncserver -kill :1 && rm -rf /tmp/.X1-lock /tmp/.X11-unix/X1"
log ""
log "${ORANGE}" "VNC Client Setup"
log "${YELLOW}" "For first-time setup, run:"
log "${WHITE}" "sudo apt install tigervnc-viewer && sudo ufw allow 5901/tcp"
log ""
log "${YELLOW}" "To connect to the VNC server:"
log "${WHITE}" "vncviewer $SERVER_IP:1"
log ""

# ----------------------------------------------
# Nginx Proxy
# ----------------------------------------------
log "${ORANGE}" "Installing Nginx server..."
sudo apt install nginx

log "${YELLOW}" "Creating SSL directory and self-signed certificate..."
sudo mkdir -p "$SSL_DIR"

sudo openssl req -x509 -nodes -days 3650 -newkey rsa:2048 \
  -keyout "$SSL_DIR/key.pem" \
  -out "$SSL_DIR/cert.pem" \
  -subj "/C=DE/ST=North Rhein Westphalia/L=Bergheim/O=MYBOTSHOP/CN=$DOMAIN_NAME" \
  -addext "subjectAltName=DNS:$DOMAIN_NAME,IP:$SERVER_IP"
log ""

log "${YELLOW}" "Writing nginx configuration..."

sudo tee "$NGINX_CONF" > /dev/null <<EOF
server {
    listen 80;
    server_name $SERVER_IP $DOMAIN_NAME;

    return 301 https://\$host\$request_uri;
}

server {
    listen 443 ssl;
    server_name $SERVER_IP $DOMAIN_NAME;

    ssl_certificate $SSL_DIR/cert.pem;
    ssl_certificate_key $SSL_DIR/key.pem;

    ssl_protocols TLSv1.2 TLSv1.3;
    ssl_ciphers HIGH:!aNULL:!MD5;

    location / {
        proxy_pass $PROXY_PASS_URL;
        proxy_http_version 1.1;
        proxy_set_header Host \$host;
        proxy_set_header X-Real-IP \$remote_addr;
        proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto \$scheme;
        proxy_set_header Upgrade \$http_upgrade;
        proxy_set_header Connection "upgrade";
    }
}
EOF

log "${YELLOW}" "Enabling site, Testing, and Reloading..."
if [ ! -f "/etc/nginx/sites-enabled/go2_webserver" ]; then
    sudo ln -s "$NGINX_CONF" /etc/nginx/sites-enabled/
else
    log "${ORANGE}" "Symbolic link already exists. Skipping."
fi
sudo nginx -t
sudo systemctl reload nginx

log "${YELLOW}" "Setup complete. Access your site at https://$SERVER_IP and https://$DOMAIN_NAME"
log "${YELLOW}" "Note: Your browser may warn about the self-signed certificate. You can safely ignore this warning for now."

# Once DNS works and port 80 is reachable from the internet, you can replace the self-signed cert with a real one:
# sudo apt install certbot python3-certbot-nginx
# sudo certbot --nginx -d $DOMAIN_NAME -d www.$DOMAIN_NAME

# ----------------------------------------------
# Add Domain to /etc/hosts
# ----------------------------------------------
sudo bash -c "
  if grep -q '$DOMAIN_NAME' '$HOSTS_FILE'; then
    echo '$DOMAIN_NAME already exists in the hosts file.'
  else
    echo '' >> '$HOSTS_FILE'
    echo '$SERVER_IP    $DOMAIN_NAME' >> '$HOSTS_FILE'
    echo 'Added $DOMAIN_NAME -> $SERVER_IP to the hosts file.'
  fi
"
