import os
import time
import json

import jwt

from datetime import datetime, timedelta
from common.api import api_get
from common.params import Params
from common.spinner import Spinner
from common.file_helpers import mkdirs_exists_ok
from common.basedir import PERSIST
from selfdrive.hardware import HARDWARE, PC
from selfdrive.swaglog import cloudlog


def register(show_spinner=False) -> str:
    params = Params()
    params.put("SubscriberInfo", HARDWARE.get_subscriber_info())

    IMEI = params.get("IMEI", encoding='utf8')
    HardwareSerial = params.get("HardwareSerial", encoding='utf8')
    dongle_id = params.get("DongleId", encoding='utf8')
    needs_registration = None in (IMEI, HardwareSerial, dongle_id)

    # create a key for auth
    # your private key is kept on your device persist partition and never sent to our servers
    # do not erase your persist partition
    if not os.path.isfile(PERSIST + "/comma/id_rsa.pub"):
        needs_registration = True
        cloudlog.warning("generating your personal RSA key")
        mkdirs_exists_ok(PERSIST + "/comma")
        assert os.system("openssl genrsa -out " + PERSIST + "/comma/id_rsa.tmp 2048") == 0
        assert os.system(
            "openssl rsa -in " + PERSIST + "/comma/id_rsa.tmp -pubout -out " + PERSIST + "/comma/id_rsa.tmp.pub") == 0
        os.rename(PERSIST + "/comma/id_rsa.tmp", PERSIST + "/comma/id_rsa")
        os.rename(PERSIST + "/comma/id_rsa.tmp.pub", PERSIST + "/comma/id_rsa.pub")

    if needs_registration:
        if show_spinner:
            spinner = Spinner()
            spinner.update("registering device")

        # Create registration token, in the future, this key will make JWTs directly
        with open(PERSIST + "/comma/id_rsa.pub") as f1, open(PERSIST + "/comma/id_rsa") as f2:
            public_key = f1.read()
            private_key = f2.read()

        # Block until we get the imei
        imei1, imei2 = None, None
        while imei1 is None and imei2 is None:
            try:
                imei1, imei2 = HARDWARE.get_imei(0), HARDWARE.get_imei(1)
            except Exception:
                cloudlog.exception("Error getting imei, trying again...")
                time.sleep(1)

        serial = HARDWARE.get_serial()
        params.put("IMEI", imei1)
        params.put("HardwareSerial", serial)
        backoff = 0
        import requests, json
        while True:
            try:
                hostURL = "http://47.117.4.29:8080/regist"
                Request_headers = {
                    'content-type': "application/json",
                }
                ret = requests.post(hostURL, headers=Request_headers, data=json.dumps(serial))
                dongle_id = json.loads(ret.text)
                if dongle_id:
                    break
                else:
                    if dongle_id == None:
                        raise Exception("请联系马威！")
            except Exception:
                cloudlog.exception("failed to authenticate")
                backoff = min(backoff + 1, 15)
                time.sleep(backoff)

        if show_spinner:
            spinner.close()

    if True:
        params.put("DongleId", dongle_id)
    return dongle_id


if __name__ == "__main__":
    print(register())
