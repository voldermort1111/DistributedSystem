package resources;

public class ThreadedWorkerWithoutSync extends Thread {
	private ResourcesExploiter rExp;

	public ThreadedWorkerWithoutSync(ResourcesExploiter resource) {
		this.rExp = resource;
	}

	public void run() {
		for (int i = 0; i < 1000; i++) {
			rExp.exploit();
		}
	}
}
