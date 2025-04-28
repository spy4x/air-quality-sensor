import { readdirSync, readFileSync } from 'fs';
import { extname, join } from 'path';
import { sql } from './lib';

const migrationsFolder: string = './migrations';
const fileExtension: string = '.sql';

async function applyMigrations(): Promise<void> {
  try {
    const tableExists: { exists: boolean }[] =
      await sql`SELECT exists (SELECT FROM information_schema.tables WHERE  table_name = 'migrations')`;

    if (!tableExists[0].exists) {
      await sql`
                CREATE TABLE migrations (
                    id SERIAL PRIMARY KEY,
                    name VARCHAR(100) NOT NULL,
                    created_at TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP
                )
            `;
      console.log('Migrations table was created.\n');
    }
  } catch (err) {
    console.error('❌ Failed to create migrations table', err);
    process.exit(1);
  }

  let migrations: string[] = [];

  try {
    const migrationsResult: any = await sql`SELECT name FROM migrations`;
    migrations = migrationsResult.map(({ name }: { name: string }) => name);
  } catch (err) {
    console.error('❌ Failed to get migrations from database', err);
    process.exit(1);
  }

  try {
    const files: string[] = readdirSync(migrationsFolder);

    const migrationsToApply: string[] = files
      .filter((file: string) => extname(file) === fileExtension)
      .map((file: string) => file.replace(fileExtension, ''))
      .filter((file: string) => !migrations.includes(file))
      .sort();

    if (!migrationsToApply.length) {
      console.log('✅ No new migrations to apply.');
      process.exit(0);
    }
    console.log(`Applying ${migrationsToApply.length} migration(s):`);

    for (let migration of migrationsToApply) {
      console.log('- ' + migration);
      const sqlScript: string = readFileSync(
        join(migrationsFolder, migration + fileExtension),
        'utf8'
      );
      await sql.begin(async (tx: any) => {
        await tx.unsafe(sqlScript);
        await tx`INSERT INTO migrations (name) VALUES (${migration})`;
      });
    }

    console.log('\n✅ Migrations successfully applied.');
    process.exit(0);
  } catch (err) {
    console.error('\n❌ Failed to apply migrations\n', err);
    process.exit(1);
  }
}

void applyMigrations();
