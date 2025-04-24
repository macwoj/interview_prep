
# Deployment strategies 

- **Recreate**  
  Stops the old version entirely, then deploys the new one. Simple but causes downtime.

- **Rolling**  
  Gradually replaces old instances with new ones. Reduces downtime, but rollback can be complex.

- **Blue-Green**  
  Runs two identical environments (blue = current, green = new). Switch traffic to green when ready. Easy rollback by switching back.

- **Canary**  
  Releases new version to a small subset of users first. If stable, gradually increase rollout. Minimizes risk.

- **A/B Testing**  
  Like canary, but segments users explicitly to test performance/UX variations before full deployment.

- **Shadow**  
  New version receives real traffic in parallel, but responses are not served to users. Good for testing behavior without impact.

- **Rolling with Pause**  
  Like rolling, but introduces pauses between steps for manual or automated checks.

Each strategy balances trade-offs:
- **Risk** (blue-green/canary minimize it)
- **Speed** (recreate is fastest)
- **Cost** (blue-green can double infra temporarily)
- **Observability** (canary/A-B allow monitoring)

## Deployment Strategies in Kubernetes:

- **Recreate**  
  In Kubernetes: `spec.strategy.type: Recreate`  
  - All old pods are killed, then new pods are started  
  - Causes downtime  
  - Rarely used in production

- **Rolling Update** (default in Kubernetes)  
  In Kubernetes: `spec.strategy.type: RollingUpdate`  
  - Updates pods gradually, maintaining service availability  
  - Controlled by `maxSurge` and `maxUnavailable`  
  - Example:
    ```yaml
    strategy:
      type: RollingUpdate
      rollingUpdate:
        maxSurge: 1
        maxUnavailable: 0
    ```

- **Blue-Green**  
  Implemented manually using two Deployments or namespaces (e.g., `my-app-blue` and `my-app-green`)  
  - Use a Service to switch traffic between them  
  - Good for easy rollback and testing before cutover

- **Canary**  
  Implemented with multiple Deployments and traffic splitting  
  - For example, run `my-app-v1` and `my-app-v2`, and use:
    - Service selectors
    - Ingress with weights (NGINX, Istio, Linkerd, etc.)
    - Or service mesh to route small % of traffic to the canary

- **A/B Testing**  
  Like canary, but user-based routing  
  - Requires HTTP headers or cookies for user segmentation  
  - Needs advanced ingress controller or service mesh (Istio is good here)

- **Shadow**  
  Not natively supported by Kubernetes alone  
  - Use Istio or Envoy to duplicate traffic to a shadow service without impacting real users  
  - Useful for validating backend changes or ML models
