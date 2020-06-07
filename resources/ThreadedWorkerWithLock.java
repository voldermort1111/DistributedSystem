package resources;

public class ThreadedWorkerWithLock extends Thread {
	private ResourcesExploiterWithLock rExp;

	public ThreadedWorkerWithLock(ResourcesExploiterWithLock resource) {
		this.rExp = resource;
	}

	public void run() {
		for (int i = 0; i < 1000; i++) {
			rExp.exploit();
		}
	}
}
