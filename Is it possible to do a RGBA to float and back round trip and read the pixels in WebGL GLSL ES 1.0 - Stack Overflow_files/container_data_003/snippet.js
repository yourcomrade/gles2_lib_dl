﻿;(()=>{"use strict";const e=Object.assign({},{"lib":"https://cdn.sstatic.net/clc/js/bundles/snippet/snippet.bundle.823.60bde180758b119646db.min.js","style":"https://cdn.sstatic.net/clc/styles/clc.min.css?v=966dc90551fb","u":"https://clc.stackoverflow.com/markup_single.js"});function t(e,t={}){const n=document.createElement(e),s=t.style||{};return delete t.style,Object.assign(n,t),Object.assign(n.style,s),n}var n=(e=>(e[e.Primary=0]="Primary",e[e.JobHeader=1]="JobHeader",e[e.JobFooter=2]="JobFooter",e[e.CompanyPageJob=3]="CompanyPageJob",e[e.CompanyPageUnfollow=-6]="CompanyPageUnfollow",e[e.CompanyPageFollow=-5]="CompanyPageFollow",e[e.CompanyPageUndismiss=-4]="CompanyPageUndismiss",e[e.CompanyPageDismiss=-3]="CompanyPageDismiss",e[e.JobUndismiss=-2]="JobUndismiss",e[e.JobDismiss=-1]="JobDismiss",e))(n||{});const s="Oops! Something went wrong. Don't worry, our best people are on it!";class i extends Error{constructor(e,t){super("Error received from server"),this.statusCode=e,this.responseBody=t}}async function o(e,t){const n="string"==typeof t?t:(e=>e?Object.keys(e).map(t=>((e,t)=>`${encodeURIComponent(e)}=${encodeURIComponent(t)}`)(t,e[t])).join("&"):void 0)(t),s=await fetch(e,{method:"POST",credentials:"include",mode:"cors",body:n});if(s.ok)return await s.json();{const e=await s.text();throw new i(s.status,e)}}const a={[n.JobDismiss]:d,[n.JobUndismiss]:u,[n.CompanyPageDismiss]:d,[n.CompanyPageUndismiss]:u,[n.CompanyPageFollow]:m,[n.CompanyPageUnfollow]:m},c=e=>(t,n=!0)=>{t.classList[n?"add":"remove"](e)},r=c("clc-hidden"),l=c("spinner");async function d(e,n,i){if(-1===i.className.indexOf("error"))try{let n=i.previousElementSibling;if(n||(n=i.parentNode.querySelector(".clc-ad-body").parentElement),!n)return;r(i.querySelector(".clc-tooltip")),l(i);const s=await o(e);l(i,!1),r(i),function(e,n){e.classList.add("clc-dismissed-container");const s=t("div",{className:"clc-dismissed-overlay clc-hidden",innerHTML:n.content});e.appendChild(s),setTimeout(()=>{r(s,!1)},20)}(n,s)}catch(e){l(i,!1),i.classList.add("error"),i.title=s}}async function u(e,t,n){let i=n;for(;!i.classList.contains("clc-dismissed-container");)i=i.parentNode;if(!i)return;const a=i.querySelector(".clc-dismissed-overlay");try{let t=i.querySelector(".clc-dismiss-icon");t||(t=i.nextElementSibling);const n=t.querySelector(".clc-tooltip"),s=a.querySelector(".clc-spinner");if(s.classList.contains("error"))return;r(s,!1),await o(e),r(s),await function(e){return new Promise(t=>{e.addEventListener("transitionend",function n(){e.removeEventListener("transitionend",n),e.parentNode.removeChild(e),t()},!1),r(e)})}(a),i.classList.remove("clc-dismissed-container"),r(t,!1),r(n,!1)}catch(e){a.classList.add("error"),a.innerHTML=`<div>${s}</div>`}}async function m(e,t,n){try{let t=n.parentElement;n.setAttribute("disabled","");const s=await o(e);if(s.content)t.innerHTML=s.content;else if(s.redirectToSignup){const e="window-location",t="/users/signup?returnUrl=";if(window!==window.parent)return void window.parent.postMessage(`${e}|${t}`,"*");window.location.href=t+window.location.pathname}var i="string"==typeof(a="clc-follow-popover")?document.getElementById(a):a;i&&i.remove()}catch(e){n.removeAttribute("disabled"),n.classList.add("error"),n.title=s}var a}globalThis.snippet=new class{constructor(){this.init()}place(e){const t=e.zones[Object.keys(e.zones)[0]].cn;if(!t)throw new Error("No creative found in response");{e.isDarkMode&&document.getElementsByTagName("body")[0].classList.add("theme-dark");const s=cam.gamClickThroughUrl;var n=document.createElement("div");n.innerHTML=t,[...Array.from(n.querySelectorAll("a"))].forEach(e=>{var t=e.getAttribute("data-clc-click"),n=parseInt(e.getAttribute("data-clc-meta-target"),10);t&&(isNaN(n)||n>=0)&&(e.href=s+t,e.setAttribute("data-clc-click-original",t),e.setAttribute("data-clc-click",s+t))}),n.innerHTML,this.setupMetaClicksForAd(n),document.body.appendChild(n),this.checkIfAllImagesLoaded()}}setupMetaClicksForAd(e){var t=window.StackExchange||{};function n(e,n){void 0===t||void 0===t.ga||"function"!=typeof t.ga.track||t.ga.track("Jobs Traffic",e,n,{nonInteraction:!1})}function s(e){let t=e.target;for(;"A"!==t.tagName&&t!==e.currentTarget;)t=t.parentNode;return t!==e.currentTarget?t:null}e.addEventListener("mousedown",function(e){const t=s(e);if(null==t)return;const n=t.dataset;n.clcClick&&(n.clcMetaTarget||(n.clcTargetUrl=t.href,t.href=n.clcClick))}),e.addEventListener("click",function(e){const t=s(e);if(null==t)return;const n=t.dataset;if(!n.clcClick||!n.clcMetaTarget)return;e.preventDefault();const i=parseInt(n.clcMetaTarget,10);if(i>=0)return;const o=a[i];"function"==typeof o&&o(n.clcClick,e,t)}),e.addEventListener("click",function(e){const t=s(e);if(null==t)return;const i=t.getAttribute("data-clc-target-url");if(!i)return;const o=e.currentTarget;o.classList.contains("js-clc-jobs-clk")||o.classList.contains("clc-jobs-multi")?n("Ad Paid | Jobs",i):o.classList.contains("clc-cp-container")?n("Ad Paid | Company",i):o.classList.contains("clc-cp-multi")&&n("Ad Paid | Shared Company Ad",i)})}init(){window.addEventListener("message",e=>{this.onmessage(e)}),window&&window.parent.postMessage("get-omni-","*")}onmessage(e){if(e.data&&"string"==typeof e.data)if(0===e.data.indexOf("omni|")){e.data;let t=e.data.split("|");this._omni=t[1],this._prg=t[2]||(window.cam||{}).prg,this.getAd()}else if(0===e.data.indexOf("markup|")){let t=e.data.indexOf("|")+1,n=e.data.slice(t);this.place(JSON.parse(n))}}getAd(){let n=window.cam||{},s=this.createMarkupUrl({u:e.u,ati:n.adTypeId,cv:n.creativeVariant,cpi:n.companyPageId,gctu:n.gamClickThroughUrl,au:n.adUnit,pv:n.productVariant,omni:this._omni,prg:this._prg,expid:n.expid},n.testOverrides||{});!function(e){const n=t("link",{rel:"stylesheet",href:e,type:"text/css"});document.head.appendChild(n)}(e.style),function(e,n=null,s=null){const i=t("script",{src:e});n&&i.addEventListener("load",()=>n(i)),s&&i.addEventListener("error",()=>s(i)),document.body.appendChild(i)}(s,null,()=>{window.parent.postMessage("collapse-","*")})}createMarkupUrl(e,t){let n=Object.keys(e).filter(t=>null!=e[t]&&null!=e[t]).reduce((t,n)=>({...t,[n]:e[n]}),{});const s=new URLSearchParams(n);if(s.delete("u"),0===Object.keys(t).length)return`${e.u}?${s}`;{const n=new URLSearchParams(Object.keys(t).reduce((e,n)=>({...e,[n]:t[n]}),{}));return`${e.u}?${s}&${n}`}}checkIfAllImagesLoaded(){let e=document.images,t=e.length,n=0;function s(){if(n++,n===t){const e=600,t=document.body.scrollHeight;let n=0;if(t!=e&&(n=t),n>0){let e=["resize",n];window.parent.postMessage(e.join("|"),"*")}}}[].forEach.call(e,function(e){e.complete?s():e.addEventListener("load",s,!1)})}}})();