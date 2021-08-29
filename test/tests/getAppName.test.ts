import Transport from '@ledgerhq/hw-transport-node-speculos';
import Hive from '@engrave/ledger-app-hive';
import { expect } from 'chai';

describe('Get app name', async () => {

    it('should get app name', async () => {
        const transport = await Transport.open({ apduPort: 40000 });
        try {
            const hive = new Hive(transport);
            const name = await hive.getAppName();
            expect(name).to.be.equal('Hive');
        } finally {
            await transport.close();
        }
    })
})