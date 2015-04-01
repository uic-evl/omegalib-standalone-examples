INSTRUCTIONS: Building an external omegalib/OSG application

You will need
- a version of omegalib built on the target machine
- cmake (you need this to build omegalib, so it will be available already)

COMMANDS:
> cmake ./ -DOmegalib_DIR="path-to-omegalib-build-directory"
> make