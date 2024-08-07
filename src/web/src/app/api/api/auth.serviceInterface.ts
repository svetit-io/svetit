/**
 * Svetit MS Project
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * The version of the OpenAPI document: 1.0.0
 * 
 *
 * NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */
import { HttpHeaders }                                       from '@angular/common/http';

import { Observable }                                        from 'rxjs';

import { UserInfo } from '../model/models';
import { UserInfos } from '../model/models';


import { Configuration }                                     from '../configuration';



export interface AuthServiceInterface {
    defaultHeaders: HttpHeaders;
    configuration: Configuration;

    /**
     * Login callback
     * 
     * @param redirectPath Redirect path
     * @param state State OIDC param
     * @param code Code OIDC param
     * @param userAgent User Agent
     * @param xForwardedProto Forwarded protocol
     * @param xForwardedHost Forwarded host
     * @param xApiPrefix Api prefix
     */
    handlerLoginCallbackGet(redirectPath: string, state: string, code: string, userAgent: string, xForwardedProto?: string, xForwardedHost?: string, xApiPrefix?: string, extraHttpRequestParams?: any): Observable<{}>;

    /**
     * Redirect to login url
     * 
     * @param redirectPath Redirect path
     * @param referer Referer
     * @param xForwardedProto Forwarded protocol
     * @param xForwardedHost Forwarded host
     * @param xApiPrefix Api prefix
     */
    handlerLoginGet(redirectPath?: string, referer?: string, xForwardedProto?: string, xForwardedHost?: string, xApiPrefix?: string, extraHttpRequestParams?: any): Observable<{}>;

    /**
     * Logout callback
     * 
     * @param xForwardedProto Forwarded protocol
     * @param xForwardedHost Forwarded host
     * @param xApiPrefix Api prefix
     */
    handlerLogoutCallbackGet(xForwardedProto?: string, xForwardedHost?: string, xApiPrefix?: string, extraHttpRequestParams?: any): Observable<{}>;

    /**
     * Redirect to logout url
     * 
     * @param xForwardedProto Forwarded protocol
     * @param xForwardedHost Forwarded host
     * @param xApiPrefix Api prefix
     * @param session cookie with session token
     */
    handlerLogoutGet(xForwardedProto?: string, xForwardedHost?: string, xApiPrefix?: string, session?: string, extraHttpRequestParams?: any): Observable<{}>;

    /**
     * Introspect token
     * 
     * @param userAgent User Agent
     * @param session cookie with session token
     */
    handlerTokenIntrospectGet(userAgent: string, session?: string, extraHttpRequestParams?: any): Observable<{}>;

    /**
     * Refresh tokens
     * 
     * @param xSession Session token
     * @param userAgent User Agent
     */
    handlerTokenRefreshPost(xSession: string, userAgent: string, extraHttpRequestParams?: any): Observable<{}>;

    /**
     * Get user info by his id
     * 
     * @param xSession Session token
     * @param id User Id path param
     */
    handlerUserByidGet(xSession: string, id: string, extraHttpRequestParams?: any): Observable<UserInfo>;

    /**
     * Get user info
     * 
     * @param xSession Session token
     */
    handlerUserInfoGet(xSession: string, extraHttpRequestParams?: any): Observable<UserInfo>;

    /**
     * List of users
     * 
     * @param xSession Session token
     * @param start Offset position
     * @param limit How many items to return at one time (max 1000)
     * @param search Param for search users by login or part of it
     */
    handlerUserListGet(xSession: string, start: number, limit: number, search?: string, extraHttpRequestParams?: any): Observable<UserInfos>;

}
