import aiohttp, asyncio, argparse, os
parser = argparse.ArgumentParser()
parser.add_argument('github')

async def main():
    async with aiohttp.ClientSession() as session:
        async with session.get(f'https://api.github.com/repos/{os.getenv("GITHUB_REPOSITORY")}/actions/runs?per_page=100&page=1') as response:
            for _ in (await response.json()).get('workflow_runs'):
                async with session.delete(f'https://api.github.com/repos/{os.getenv("GITHUB_REPOSITORY")}/actions/runs/{_.get("id")}', auth=aiohttp.BasicAuth('chaowenguo', parser.parse_args().github)) as _: pass

asyncio.run(main())
