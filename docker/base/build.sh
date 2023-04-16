cp /etc/apt/trusted.gpg.d/jetson-ota-public.asc ../.. # copy to old_jetbot root

sudo docker build \
    --build-arg BASE_IMAGE=$JETBOT_BASE_IMAGE \
    -t $JETBOT_DOCKER_REMOTE/jetbot:base-$JETBOT_VERSION-$L4T_VERSION \
    -f Dockerfile \
    ../..  # old_jetbot repo root as context

