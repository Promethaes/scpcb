Function InitializeNPCtypeApache(n.NPCs)
    n\NVName = "Human"
    n\GravityMult = 0.0
    n\MaxGravity = 0.0
    n\Collider = CreatePivot()
    EntityRadius n\Collider, 0.2
    n\obj = CopyEntity(ApacheObj);LoadAnimMesh_Strict("GFX\apache.b3d")
    
    n\obj2 = CopyEntity(ApacheRotorObj);LoadAnimMesh_Strict("GFX\apacherotor.b3d",n\obj)
    EntityParent n\obj2,n\obj
    
    For i = -1 To 1 Step 2
        Local rotor2 = CopyEntity(n\obj2,n\obj2)
        RotateEntity rotor2,0,4.0*i,0
        EntityAlpha rotor2, 0.5
    Next
    
    n\obj3 = LoadAnimMesh_Strict("GFX\apacherotor2.b3d",n\obj)
    PositionEntity n\obj3, 0.0, 2.15, -5.48
    
    EntityType n\Collider, HIT_APACHE
    EntityRadius n\Collider, 3.0
    
    For i = -1 To 1 Step 2
        Local Light1 = CreateLight(2,n\obj)
        ;room\LightDist[i] = range
        LightRange(Light1,2.0)
        LightColor(Light1,255,255,255)
        PositionEntity(Light1, 1.65*i, 1.17, -0.25)
        
        Local lightsprite = CreateSprite(n\obj)
        PositionEntity(lightsprite, 1.65*i, 1.17, 0, -0.25)
        ScaleSprite(lightsprite, 0.13, 0.13)
        EntityTexture(lightsprite, LightSpriteTex(0))
        EntityBlend (lightsprite, 3)
        EntityFX lightsprite, 1+8
    Next
    
    temp# = 0.6
    ScaleEntity n\obj, temp, temp, temp
End Function

Function UpdateNPCtypeApache(n.NPCs)
    Local dist# = EntityDistance(Collider, n\Collider)
    If dist<60.0 Then 
        If PlayerRoom\RoomTemplate\Name = "exit1" Then 
            dist2 = Max(Min(EntityDistance(n\Collider, PlayerRoom\Objects[3])/(8000.0*RoomScale),1.0),0.0)
        Else 
            dist2 = 1.0
        EndIf
        
        n\SoundChn = LoopSound2(ApacheSFX, n\SoundChn, Camera, n\Collider, 25.0, dist2)
    EndIf
    
    n\DropSpeed = 0
    
    Select n\State
        Case 0,1
            TurnEntity(n\obj2,0,20.0*FPSfactor,0)
            TurnEntity(n\obj3,20.0*FPSfactor,0,0)
            
            If n\State=1 And (Not NoTarget) Then
                If Abs(EntityX(Collider)-EntityX(n\Collider))< 30.0 Then
                    If Abs(EntityZ(Collider)-EntityZ(n\Collider))<30.0 Then
                        If Abs(EntityY(Collider)-EntityY(n\Collider))<20.0 Then
                            If Rand(20)=1 Then 
                                If EntityVisible(Collider, n\Collider) Then
                                    n\State = 2
                                    PlaySound2(AlarmSFX(2), Camera, n\Collider, 50, 1.0)
                                EndIf
                            EndIf									
                        EndIf
                    EndIf
                EndIf							
            EndIf
        Case 2,3 ;player located -> attack
            
            If n\State = 2 Then 
                target = Collider
            ElseIf n\State = 3
                target=CreatePivot()
                PositionEntity target, n\EnemyX, n\EnemyY, n\EnemyZ, True
            EndIf
            
            If NoTarget And n\State = 2 Then n\State = 1
            
            TurnEntity(n\obj2,0,20.0*FPSfactor,0)
            TurnEntity(n\obj3,20.0*FPSfactor,0,0)
            
            If Abs(EntityX(target)-EntityX(n\Collider)) < 55.0 Then
                If Abs(EntityZ(target)-EntityZ(n\Collider)) < 55.0 Then
                    If Abs(EntityY(target)-EntityY(n\Collider))< 20.0 Then
                        PointEntity n\obj, target
                        RotateEntity n\Collider, CurveAngle(Min(WrapAngle(EntityPitch(n\obj)),40.0),EntityPitch(n\Collider),40.0), CurveAngle(EntityYaw(n\obj),EntityYaw(n\Collider),90.0), EntityRoll(n\Collider), True
                        PositionEntity(n\Collider, EntityX(n\Collider), CurveValue(EntityY(target)+8.0,EntityY(n\Collider),70.0), EntityZ(n\Collider))
                        
                        dist# = Distance(EntityX(target),EntityZ(target),EntityX(n\Collider),EntityZ(n\Collider))
                        
                        n\CurrSpeed = CurveValue(Min(dist-6.5,6.5)*0.008, n\CurrSpeed, 50.0)
                        
                        ;If Distance(EntityX(Collider),EntityZ(Collider),EntityX(n\collider),EntityZ(n\collider)) > 6.5 Then
                        ;	n\currspeed = CurveValue(0.08,n\currspeed,50.0)
                        ;Else
                        ;	n\currspeed = CurveValue(0.0,n\currspeed,30.0)
                        ;EndIf
                        MoveEntity n\Collider, 0,0,n\CurrSpeed*FPSfactor
                        
                        
                        If n\PathTimer = 0 Then
                            n\PathStatus = EntityVisible(n\Collider,target)
                            n\PathTimer = Rand(100,200)
                        Else
                            n\PathTimer = Min(n\PathTimer-FPSfactor,0.0)
                        EndIf
                        
                        If n\PathStatus = 1 Then ;player visible
                            RotateEntity n\Collider, EntityPitch(n\Collider), EntityYaw(n\Collider), CurveAngle(0, EntityRoll(n\Collider),40), True
                            
                            If n\Reload =< 0 Then
                                If dist<20.0 Then
                                    pvt = CreatePivot()
                                    
                                    PositionEntity pvt, EntityX(n\Collider),EntityY(n\Collider), EntityZ(n\Collider)
                                    RotateEntity pvt, EntityPitch(n\Collider), EntityYaw(n\Collider),EntityRoll(n\Collider)
                                    MoveEntity pvt, 0, 8.87*(0.21/9.0), 8.87*(1.7/9.0) ;2.3
                                    PointEntity pvt, target
                                    
                                    If WrapAngle(EntityYaw(pvt)-EntityYaw(n\Collider))<10 Then
                                        PlaySound2(Gunshot2SFX, Camera, n\Collider, 20)
                                        
                                        DeathMSG = Chr(34)+"CH-2 to control. Shot down a runaway Class D at Gate B."+Chr(34)
                                        
                                        Shoot( EntityX(pvt),EntityY(pvt), EntityZ(pvt),((10/dist)*(1/dist))*(n\State=2),(n\State=2))
                                        
                                        n\Reload = 5
                                    EndIf
                                    
                                    FreeEntity pvt
                                EndIf
                            EndIf
                        Else 
                            RotateEntity n\Collider, EntityPitch(n\Collider), EntityYaw(n\Collider), CurveAngle(-20, EntityRoll(n\Collider),40), True
                        EndIf
                        MoveEntity n\Collider, -EntityRoll(n\Collider)*0.002,0,0
                        
                        n\Reload=n\Reload-FPSfactor
                        
                        
                    EndIf
                EndIf
            EndIf		
            
            If n\State = 3 Then FreeEntity target
        Case 4 ;crash
            If n\State2 < 300 Then
                
                TurnEntity(n\obj2,0,20.0*FPSfactor,0)
                TurnEntity(n\obj3,20.0*FPSfactor,0,0)
                
                TurnEntity n\Collider,0,-FPSfactor*7,0;Sin(MilliSecs2()/40)*FPSfactor
                n\State2=n\State2+FPSfactor*0.3
                
                target=CreatePivot()
                PositionEntity target, n\EnemyX, n\EnemyY, n\EnemyZ, True
                
                PointEntity n\obj, target
                MoveEntity n\obj, 0,0,FPSfactor*0.001*n\State2
                PositionEntity(n\Collider, EntityX(n\obj), EntityY(n\obj), EntityZ(n\obj))
                
                If EntityDistance(n\obj, target) <0.3 Then
                    If TempSound2 <> 0 Then FreeSound_Strict TempSound2 : TempSound2 = 0
                    TempSound2 = LoadSound_Strict("SFX\Character\Apache\Crash"+Rand(1,2)+".ogg")
                    CameraShake = Max(CameraShake, 3.0)
                    PlaySound_Strict TempSound2
                    n\State = 5
                EndIf
                
                FreeEntity target
            EndIf
    End Select
    
    PositionEntity(n\obj, EntityX(n\Collider), EntityY(n\Collider), EntityZ(n\Collider))
    RotateEntity n\obj, EntityPitch(n\Collider), EntityYaw(n\Collider), EntityRoll(n\Collider), True
End Function