#include "Radio.h"
#include "include.h"

namespace CBN {

// Structs.
std::vector<Radio*> Radio::list;
Radio::Radio() {
    list.push_back(this);
}
Radio::~Radio() {
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == this) {
            list.erase(list.begin() + i);
            break;
        }
    }
}
int Radio::getListSize() {
    return list.size();
}
Radio* Radio::getObject(int index) {
    return list[index];
}

// Constants.
const int RADIO_CHANNEL_COUNT = 5;

// Globals.
Radio* radio = nullptr;

// Functions.
Radio* CreateRadio() {
    Radio* rad = new Radio();
    return rad;
}

void ChangeRadioChannel(int newChn) {
    if (IsChannelPlaying(radio->channels[radio->currChn])) {
        bbPauseChannel(radio->channels[radio->currChn]);
    }

    if (!IsChannelPlaying(radio->channels[newChn])) {
        radio->channels[newChn] = bbPlaySound(radio->sndStatic);
    } else {
        bbResumeChannel(radio->channels[newChn]);
    }

    radio->currChn = newChn;
}

void UpdateRadio(Item* i) {
    switch (radio->currChn) {
        case 0: {
            //TODO: something?
            //-alarms and alert messages
        }
        case 1: {

            //-scp on-site radio
        }
        case 2: {

            //-MTF broadcasts
        }
        case 3: {

            //-idfk
        }
        case 4: {
        }
    }

    radio->airTime[radio->currChn] = radio->airTime[radio->currChn] + (timing->tickDuration/70);
}

}