external shared class Zone;

class EntranceZone : Zone {
    void generate() {
        rooms = array<array<Room@>>(10);
        for (int x=0;x<10;x++) {
            rooms[x] = array<Room@>(10);
            for (int y=0;y<10;y++) {
                if (y % 2 == 0) {
                    if (x % 2 == 0) {
                        @rooms[x][y] = mapGenEntries[0].roomConstructor(this);
                    } else {
                        @rooms[x][y] = mapGenEntries[1].roomConstructor(this);
                        rooms[x][y].rotation = 90;
                    }
                } else {
                    if (x % 2 == 0) {
                        @rooms[x][y] = mapGenEntries[1].roomConstructor(this);
                    } else {
                        // Do nothing
                    }
                }
                if (rooms[x][y] != null) {
                    rooms[x][y].position = Vector3f(x*204.8,0,y*204.8);
                    if (rooms[x][y].rotation == 0) { rooms[x][y].rotation = 0; }

                    RM2@ mesh = rooms[x][y].mesh;
                    Matrix4x4f worldMatrix = rooms[x][y].worldMatrix;
                    for (int i=0;i<mesh.collisionMeshCount();i++) {
                        testCollCollection.addInstance(mesh.getCollisionMesh(i), worldMatrix);
                    }
                }
            }
        }
    }
}
