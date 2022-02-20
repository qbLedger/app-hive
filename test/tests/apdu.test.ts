import Transport from '@ledgerhq/hw-transport-node-speculos';
import assert from 'assert';


describe('APDU', async () => {

    it('should reject unsupported instruction', async () => {
        const transport = await Transport.open({ apduPort: 40000 });
        try {
            await transport.send(0xD4, 0x16, 0x00, 0x00);
        } catch (error: any) {
            assert.equal(error.statusCode, 0x6D00); //SW_INS_NOT_SUPPORTED
        }
        finally {
            await transport.close();
        }
    })

    it('should reject unsupported CLA', async () => {
        const transport = await Transport.open({ apduPort: 40000 });
        try {
            await transport.send(0xD6, 0x06, 0x00, 0x00);
        } catch (error: any) {
            assert.equal(error.statusCode, 0x6E00); //SW_CLA_NOT_SUPPORTED
        }
        finally {
            await transport.close();
        }
    })
})