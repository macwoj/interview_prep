- https://github.com/mxssl/sre-interview-prep-guide

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

# Determining which server processed a specific request

### 1. **Request ID or Trace ID**
- Assign a unique ID (e.g. UUID or hash) to each request at the entry point (API gateway, load balancer).
- Propagate this ID through headers (e.g. `X-Request-ID`, `traceparent`).
- Each server logs the ID along with its own hostname or instance ID.
- Use logs or tracing tools to trace the request through the system.

### 2. **Structured Logging**
- Include metadata like:
  - Hostname or instance ID
  - Service name
  - Request ID
  - Timestamp
- Centralized logging systems (e.g. ELK, Fluentd, Loki) allow querying logs to find which server handled a specific request.

### 3. **Distributed Tracing**
- Use tools like OpenTelemetry, Jaeger, Zipkin, or AWS X-Ray.
- These instruments track request flow across services and include:
  - Spans for each service or component
  - Metadata like service name, host, processing time
- You can see the full path and which nodes processed the request.

### 4. **Load Balancer or Reverse Proxy Logs**
- If your request passes through a load balancer (e.g. HAProxy, Envoy, NGINX), its access logs may contain:
  - Backend server IP
  - Request metadata
  - Response code and latency

### 5. **Custom Headers**
- Servers can inject their hostname or instance ID in the response (e.g. `X-Served-By` header).
- Useful for debugging and external monitoring.

### Example (headers):
```
X-Request-ID: a1234
X-Served-By: api-server-03
```


# Kubernetes, to determine which pod (and node) processed a request


### 1. **Inject a Request ID**
- Use a middleware or ingress controller (like NGINX Ingress or Istio) to generate a `X-Request-ID`.
- Propagate it through the request.
- Each service logs this ID along with pod metadata (e.g. pod name, namespace, node).

---

### 2. **Log Pod Metadata**
Configure your logging to include:
- Pod name: available in the environment as `HOSTNAME`
- Pod IP: `status.podIP` from the Kubernetes API
- Node name: available via downward API (mount as env var or file)
  
Example env config in pod spec:
```yaml
env:
  - name: NODE_NAME
    valueFrom:
      fieldRef:
        fieldPath: spec.nodeName
```

---

### 3. **Use a Sidecar or Interceptor (e.g. Istio, Linkerd)**
- Automatically adds tracing headers like `traceparent`, `X-Request-ID`, `b3`.
- Tracing data includes pod, node, and service.
- View traces in Jaeger, Tempo, or similar.

---

### 4. **Centralized Logging**
Use tools like:
- **Fluent Bit / Fluentd / Vector** → forward logs
- **Elasticsearch + Kibana**, **Loki + Grafana**, **Cloud-native services (GCP Logging, AWS CloudWatch)**

Filter logs by:
- `kubernetes.pod_name`
- `kubernetes.container_name`
- `kubernetes.node_name`
- `log.request_id`

---

### 5. **Custom Response Headers**
Let the pod set:
- `X-Served-By: $(POD_NAME)`
- `X-Node-Name: $(NODE_NAME)`

Use downward API to populate these in env vars.

---

### 6. **Use Kubernetes Audit Logs (optional)**
For API-level request tracking (not app-level), audit logs show which API server handled which request and what node or pod was touched.

---

Let me know if you want code examples for any of these (like NGINX Ingress, Fluent Bit, or OpenTelemetry in K8s).