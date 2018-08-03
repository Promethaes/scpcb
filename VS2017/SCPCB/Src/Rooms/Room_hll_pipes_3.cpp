#include "Room_hll_pipes_3.h"
#include "include.h"

namespace CBN {

// Functions.
void UpdateEvent106sinkhole(Event* e) {
    float dist;
    int i;
    int temp;
    int pvt;
    String strtemp;
    int j;
    int k;

    Particle* p;
    NPC* n;
    Room* r;
    Event* e2;
    Item* it;
    Emitter* em;
    SecurityCam* sc;
    SecurityCam* sc2;
    Decal* de;

    String CurrTrigger = "";

    float x;
    float y;
    float z;

    float angle;

    //[Block]
    if (e->eventState==0) {
        de = CreateDecal(DECAL_CORROSION, bbEntityX(e->room->obj)+bbRnd(-0.5,0.5), 0.01, bbEntityZ(e->room->obj)+bbRnd(-0.5,0.5), 90, bbRand(360), 0);
        //
        de->size = 2.5;
        bbScaleSprite(de->obj, de->size, de->size);

        e->eventState = 1;
    } else if ((mainPlayer->currRoom == e->room)) {
        if (e->sounds[0]==0) {
            e->sounds[0] = bbLoadSound("SFX/Room/Sinkhole.ogg");
        } else {
            e->soundChannels[0] = LoopRangedSound(e->sounds[0], e->soundChannels[0], mainPlayer->cam, e->room->obj, 4.5, 1.5);
        }
        dist = Distance(bbEntityX(mainPlayer->collider),bbEntityZ(mainPlayer->collider),bbEntityX(e->room->obj),bbEntityZ(e->room->obj));
        if (dist < 2.0) {
            mainPlayer->footstepOverride = 1;
            mainPlayer->moveSpeed = CurveValue(0.0, mainPlayer->moveSpeed, Max(dist*50,1.0));
            mainPlayer->crouchState = (2.0-dist)/2.0;

            if (dist<0.5) {
                if (e->eventState2 == 0) {
                    PlaySound2(LoadTempSound("SFX/Room/SinkholeFall.ogg"));
                }

                mainPlayer->moveSpeed = CurveValue(0.0, mainPlayer->moveSpeed, Max(dist*50,1.0));

                x = CurveValue(bbEntityX(e->room->obj),bbEntityX(mainPlayer->collider),10.0);
                y = CurveValue(bbEntityY(e->room->obj)-e->eventState2,bbEntityY(mainPlayer->collider),25.0);
                z = CurveValue(bbEntityZ(e->room->obj),bbEntityZ(mainPlayer->collider),10.0);
                bbPositionEntity(mainPlayer->collider, x, y, z, true);

                mainPlayer->dropSpeed = 0;

                bbResetEntity(mainPlayer->collider);

                e->eventState2 = Min(e->eventState2+timing->tickDuration/200.0,2.0);

                //LightBlink = Min(e\eventState2*5,10.0)
                mainPlayer->blurTimer = e->eventState2*500;

                if (e->eventState2 == 2.0) {
                    MoveToPocketDimension();
                }
            }
        }
    } else {
        e->eventState2 = 0;
    }

    //[End Block]

}

}