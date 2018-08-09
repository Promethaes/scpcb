#include <bbblitz3d.h>
#include <bbaudio.h>
#include <bbmath.h>

#include "NPCs.h"
#include "../INI.h"
#include "../GameMain.h"
#include "../Events.h"
#include "../Menus/Menu.h"
#include "../Audio.h"
#include "../MapSystem.h"
#include "../Player.h"
#include "../MathUtils/MathUtils.h"
#include "../Difficulty.h"
#include "../Objects.h"
#include "../Doors.h"
#include "../Decals.h"
#include "../Particles.h"
#include "NPCtypeGuard.h"

namespace CBN {

// Constants.
const int STATEGUARD_IDLE = 0;
const int STATEGUARD_LOOK = 1;
const int STATEGUARD_MOVE_TO_TARGET = 2;
const int STATEGUARD_SHOOT_TARGET = 3;
const int STATEGUARD_DEAD = 4;

// Functions.
void InitializeNPCtypeGuard(NPC* n) {
    n->nvName = "Humen";
    n->collider = bbCreatePivot();
    bbEntityRadius(n->collider, 0.2);
    bbEntityType(n->collider, HIT_PLAYER);

    LoadOrCopyMesh(n, "GFX/NPCs/guard/guard.b3d");

    n->speed = (GetINIFloat("Data/NPCs.ini", "Guard", "speed") / 100.0);

    float temp = (GetINIFloat("Data/NPCs.ini", "Guard", "scale") / 2.5);
    bbScaleEntity(n->obj, temp, temp, temp);

    MeshCullBox(n->obj, -bbMeshWidth(n->obj), -bbMeshHeight(n->obj), -bbMeshDepth(n->obj), bbMeshWidth(n->obj)*2, bbMeshHeight(n->obj)*2, bbMeshDepth(n->obj)*2);
}

void UpdateNPCtypeGuard(NPC* n) {
    float dist;

    Object* head;
    float headangle;
    Pivot* pvt;
    Particle* p;

    float prevFrame = n->frame;

    switch ((int)n->state) {
        case STATEGUARD_LOOK: {
            head = bbFindChild(n->obj,"head");
            headangle = bbEntityYaw(head);

            if (n->target != nullptr) {
                n->targetX = bbEntityX(n->target->collider);
                n->targetY = bbEntityY(n->target->collider);
                n->targetZ = bbEntityZ(n->target->collider);
            }

            if (headangle > -45 & headangle < 45) {
                bbPointEntity(head,n->target->collider);
            }

            AnimateNPC(n,77,201,0.2);
        }
        case STATEGUARD_MOVE_TO_TARGET: {
            bbRotateEntity(n->collider, 0, CurveAngle(bbVectorYaw(n->targetX-bbEntityX(n->collider), 0, n->targetZ-bbEntityZ(n->collider))+n->angle, bbEntityYaw(n->collider), 20.0), 0);

            dist = Distance(bbEntityX(n->collider), bbEntityZ(n->collider), n->targetX, n->targetZ);

            AnimateNPC(n, 1614, 1641, n->currSpeed * 30);

            if (dist > 2.0 | dist < 1.0 ) {
                n->currSpeed = CurveValue(n->speed * Sgn(dist - 1.5) * 0.75, n->currSpeed, 10.0);
            } else {
                n->currSpeed = CurveValue(0, n->currSpeed, 10.0);
            }

            if (n->currSpeed > 0.01) {
                if (prevFrame > 1638 & n->frame < 1620) {
                    PlayRangedSound(sndManager->footstepMetal[bbRand(0,7)]->internal, mainPlayer->cam, n->collider, 8.0, bbRnd(0.5, 0.7));
                } else if ((prevFrame < 1627 & n->frame>=1627)) {
                    PlayRangedSound(sndManager->footstepMetal[bbRand(0,7)]->internal, mainPlayer->cam, n->collider, 8.0, bbRnd(0.5, 0.7));
                }
            }

            bbMoveEntity(n->collider, 0, 0, n->currSpeed * timing->tickDuration);

        }
        case STATEGUARD_SHOOT_TARGET: {
            //Raising gun to aim animation.
            AnimateNPC(n, 1539, 1553, 0.2, false);

            pvt = bbCreatePivot();
            bbPositionEntity(pvt, n->targetX, n->targetY, n->targetZ);

            //TODO: Make relative to target.
            bbPointEntity(pvt, mainPlayer->collider);

            bbRotateEntity(pvt, Min(bbEntityPitch(pvt), 20), bbEntityYaw(pvt), 0);

            bbRotateEntity(n->collider, CurveAngle(bbEntityPitch(pvt), bbEntityPitch(n->collider), 10), CurveAngle(bbEntityYaw(pvt), bbEntityYaw(n->collider), 10), 0, true);

            //Start shooting once the aiming animation is done.
            if (n->timer < 0 & n->frame>1550) {
                PlayRangedSound_SM(sndManager->gunshot[0], mainPlayer->cam, n->collider, 35);

                bbRotateEntity(pvt, bbEntityPitch(n->collider), bbEntityYaw(n->collider), 0, true);
                bbPositionEntity(pvt, bbEntityX(n->obj), bbEntityY(n->obj), bbEntityZ(n->obj));
                bbMoveEntity(pvt,0.8*0.079, 10.75*0.079, 6.9*0.079);

                bbPointEntity(pvt, mainPlayer->collider);

                p = CreateParticle(bbEntityX(n->obj, true), bbEntityY(n->obj, true), bbEntityZ(n->obj, true), PARTICLE_FLASH, 0.2, 0.0, 5);
                bbPositionEntity(p->pvt, bbEntityX(n->obj), bbEntityY(n->obj), bbEntityZ(n->obj));
                bbRotateEntity(p->pvt, bbEntityPitch(n->collider), bbEntityYaw(n->collider), 0, true);
                bbMoveEntity(p->pvt,0.8*0.079, 10.75*0.079, 6.9*0.079);

                n->reload = 7;
            }

            bbFreeEntity(pvt);

            n->timer = n->timer - timing->tickDuration;
        }
        case STATEGUARD_DEAD: {
            if (n->frame <= 151) {
                SetNPCFrame(n,151);
            } else {
                AnimateNPC(n,113,151,0.2);
            }
        }
    }
}

}
