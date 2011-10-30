.. index::
    single: Cache; Varnish

Varnish を使ってウェブサイトを高速化する方法
============================================

Symfony2 のキャッシュは標準的な HTTP キャッシュヘッダを使用しているので、 :ref:`symfony-gateway-cache` は簡単に他のリバースプロクシに変更することができます。 Varnish は強力なオープンソースの HTTP アクセラレータで キャッシュの利用を速くすることができ、また、 :ref:`Edge Side Includes<edge-side-includes>` のサポートもしています。

.. index::
    single: Varnish; configuration

コンフィギュレーション
----------------------

前に見たように、 Symfony2 はとてもスマートで、リバースプロクシが ESI を理解できるか検知することができます。 Symfony2 のリバースプロクシを使用している際には、そのままで動作します。しかし、 Varnish を使用する際には、特別なコンフィギュレーションが必要になります。 Symfony2 は `Edge Architecture` と呼ばれる Akamai によって作られた標準に従っていますので、たとえ Symfony2 を使っていなくても、この章のコンフィギュレーションのティップスは便利です。

.. note::

    Varnish は ESI タグ(``onerror`` や ``alt`` 属性は無視されます)の ``src`` 属性のみをサポートします。

まず、 Varnish を設定します。バックエンドのアプリケーションにフォワードされたリクエストに ``Surrogate-Capability`` ヘッダ追加して ESI サポートを通知してください。

.. code-block:: text

    sub vcl_recv {
        set req.http.Surrogate-Capability = "abc=ESI/1.0";
    }

そして、Symfony2 が自動的に付与する ``Surrogate-Control`` ヘッダをチェックして ESI タグがあるときのみレスポンスの内容をパースするように Varnish を最適化します。:

.. code-block:: text

    sub vcl_fetch {
        if (beresp.http.Surrogate-Control ~ "ESI/1.0") {
            unset beresp.http.Surrogate-Control;

            // for Varnish >= 3.0
            set beresp.do_esi = true;
            // for Varnish < 3.0
            // esi;
        }
    }

.. caution::

    ESI での圧縮は Varnish のバージョンが 3.0 より下であるとサポートされていません( `GZIP and Varnish`_ を参照してください)。使用している Varnish のバージョンが 3.0 より下の際には、圧縮を使用するために Varnish の前にウェブサーバを用意してください。

.. index::
    single: Varnish; Invalidation

キャッシュの無効化
------------------

キャッシュデータは、 HTTP キャッシュモデルでネイティブに使用しているので、無効化する必要は全くありません。詳細は :ref:`http-cache-invalidation` を参照してください。

指定したキャッシュを無効化する Varnish の特別な HTTP ``PURGE`` メソッドを受け取ることができるような設定も可能です。

.. code-block:: text

    sub vcl_hit {
        if (req.request == "PURGE") {
            set obj.ttl = 0s;
            error 200 "Purged";
        }
    }

    sub vcl_miss {
        if (req.request == "PURGE") {
            error 404 "Not purged";
        }
    }

.. caution::

    誰かがランダムにキャッシュデータをパージしないように ``PURGE`` HTTP メソッドは保護してください。

.. _`Edge Architecture`: http://www.w3.org/TR/edge-arch
.. _`GZIP and Varnish`: https://www.varnish-cache.org/docs/3.0/phk/gzip.html

.. 2011/10/27 ganchiku d540e14acc8db6048279d0fed18b9403082710a9

